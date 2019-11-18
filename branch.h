#ifndef XO_BRANCH_H
#define XO_BRANCH_H

#include "variationmanager.h"
#include "variationscanner.h"
#include "statictactics.h"
#include "movegenerator.h"


namespace XO{
    class StaticTacticsAdapter : public StaticTactics, BaseEvaluator{
    public:
        void operator()(Data& links, Piece own) const override{
            Report report;
            StaticTactics()(report, links.obs, own);
            if(report.winner != EMPTY){
                assert(report.winner == ALLY || report.winner == ENEMY);
                links.result = EvaluationReport(report.m
                    , report.depth
                    , EvaluationReport::Mode::STATIC_TACTICS
                    , report.winner == ALLY ? EvaluationReport::Type::SUCCESS
                                            : EvaluationReport::Type::FAIL);
            }
        }
    };

    static void MakeDepthLimitReport(BaseEvaluator::Data& links, Piece own){
        links.result = EvaluationReport(links.mgr.GetDepthController().DepthLimit()
                , EvaluationReport::Mode::DEEP_TACTICS
                , EvaluationReport::Type::DEPTH_LIMIT);
    }

    template<typename EvAgent>
    class BranchS4 : public BaseEvaluator{
    public:
        static constexpr TProperty SquareClass(){
            return TProperty::S4;
        }

        void operator()(BaseEvaluator::Data& links, Piece own) const override{
            assert(links.obs.SquareCount(own, TProperty::WIN) == 1);
            auto blocker = links.obs.AnySquare(own, TProperty::WIN);

            auto mv = Move(blocker, OppositePiece(own));
            auto pos_ptr = links.mgr.FindPositionDataPtr(mv);
            if(!pos_ptr){
                auto v_answer = AutoVariation(links.mgr, mv);
                EvAgent()(links, own);
            }
            else{
                links.result = pos_ptr->GetReport();
            }

            if(links.result.Success()){
                links.result.moves.push_front(Move(blocker, OppositePiece(own)));
            }
        }
    };

    template<typename EvAgent>
    class Scan4SOnly : public BaseEvaluator{
    public:
        void operator()(Data& links, Piece own) const override{
            if(!P_Final<EvAgent>()(links, own)){
                if(links.mgr.AtMaxDepth()){
                    if(!MoveGenerator(links.obs, own, TProperty::S4).Empty()){
                        MakeDepthLimitReport(links, own);
                    }
                }
                else{
                    auto s4gen = MoveGenerator(links.obs, own, TProperty::S4);
                    VariationScanner<BranchS4<Scan4SOnly<EvAgent>>>()(links, own, s4gen);
                }
            }
            if(links.result.Final()){
                auto pos_ptr = links.mgr.GetPositionDataPtr(own);
                pos_ptr->SetReport(links.result);
            }
        }
    };

    template<typename EvAgent>
    class BranchD3 : public BaseEvaluator{
    public:
        static constexpr TProperty SquareClass(){
            return TProperty::D3;
        }

        void operator()(BaseEvaluator::Data& links, Piece own) const override{
            assert(!links.obs.HaveSquares(own, TProperty::WIN));
            assert(links.obs.HaveSquares(own, TProperty::D4));
            BaseEvaluator::Data counter_links(links);
            Scan4SOnly<StaticTacticsAdapter>()(counter_links, OppositePiece(own));
            if(counter_links.result.Success()){
                return;
            }

            if(counter_links.result.DepthLimit()){
                links.result = std::move(counter_links.result);
                return;   //TODO: обрывать перебор при достижении макс. глубины (false)
            }

            assert(!counter_links.result.Fail());

            EvaluationReport report_candidate;
            MoveGenerator d4_blockers(links.obs, OppositePiece(own), TProperty::ANY);
            for(; d4_blockers.Valid(); d4_blockers.Next()){
                BaseEvaluator::Data next_links{links};
                auto pos_ptr = links.mgr.FindPositionDataPtr(d4_blockers.GetMove());
                if(!pos_ptr){
                    auto v_answer = AutoVariation(links.mgr, d4_blockers.GetMove());
                    EvAgent()(next_links, own);
                }
                else{
                    next_links.result = pos_ptr->GetReport();
                }

                if(!next_links.result.Success()){
                    if(next_links.result.DepthLimit()){
                        links.result = std::move(next_links.result);
                    }
                    return;
                }

                next_links.result.moves.push_front(d4_blockers.GetMove());
                if(!report_candidate.Final()
                   || next_links.result.FullDepth() > report_candidate.FullDepth()){
                    report_candidate = std::move(next_links.result);
                }
            }

            if(d4_blockers.Empty()){
                report_candidate = EvaluationReport(
                        4
                        , EvaluationReport::Mode::DEEP_TACTICS
                        , EvaluationReport::Type::SUCCESS);
            }

            std::vector<Point> targets;
            for(MoveGenerator gen(links.obs, OppositePiece(own), TProperty::S4);
                gen.Valid(); gen.Next()){
                targets.push_back(gen.Get());
            }
            for(auto& sq: targets){
                Move mv(sq, OppositePiece(own));
                BaseEvaluator::Data blocker_links(links);
                //TODO: придумать как вынести отдельной функцией
                auto v_start = AutoVariation(links.mgr, mv);
                StaticTacticsAdapter()(blocker_links, own);
                if(blocker_links.result.Success()){
                    continue;
                }
                auto blocker = links.obs.AnySquare(OppositePiece(own), TProperty::WIN);
                auto v_answer = AutoVariation(links.mgr, Move(blocker, own));
                //ODOT:
                if(links.obs.HaveSquares(own, TProperty::WIN)){
                    BranchS4<EvAgent>()(blocker_links, own);
                }
                else if(links.obs.HaveSquares(own, TProperty::D4)){
                    BranchD3<EvAgent>()(blocker_links, own);
                }
                else{
                    return;
                }
                if(!blocker_links.result.Success()){
                    if(blocker_links.result.DepthLimit()){
                        links.result = std::move(blocker_links.result);
                    }
                    return;
                }
                else{
                    blocker_links.result.moves.push_front(v_answer);
                    blocker_links.result.moves.push_front(v_start);
                    if(blocker_links.result.FullDepth() > report_candidate.FullDepth()){
                        report_candidate = std::move(blocker_links.result);
                    }
                }
            }

            links.result = std::move(report_candidate);
        }
    };

    template<typename EvAgent>
    class Scan4S3D : public BaseEvaluator{
    public:
        void operator()(Data& links, Piece own) const override{
            if(!P_Final<EvAgent>()(links, own)){
                if(links.mgr.AtMaxDepth()){
                    if(!MoveGenerator(links.obs, own, TProperty::S4).Empty()
                     || !MoveGenerator(links.obs, own, TProperty::D3).Empty()){
                        MakeDepthLimitReport(links, own);
                    }
                }
                else{
                    auto s4gen = MoveGenerator(links.obs, own, TProperty::S4);
                    VariationScanner<BranchS4<Scan4S3D<EvAgent>>>()(links, own, s4gen);
                    if(!links.result.Final()){
                        auto d3gen = MoveGenerator(links.obs, own, TProperty::D3);
                        VariationScanner<BranchD3<Scan4S3D<EvAgent>>>()(links, own, d3gen);
                    }
                }
            }
            auto pos_ptr = links.mgr.GetPositionDataPtr(own);
            pos_ptr->SetReport(links.result);
        }
    };
}

#endif //XO_BRANCH_H
