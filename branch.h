#ifndef XO_BRANCH_H
#define XO_BRANCH_H

#include "variationmanager.h"
#include "variationscanner.h"
#include "statictactics.h"
#include "movegenerator.h"


namespace XO{
    class StaticTacticsAdapter : public StaticTactics, BaseEvaluator{
    public:
        void operator()(EvaluationReport& r_result, VariationManager& mgr, Piece own) const override{
            Report report;
            StaticTactics()(report, mgr.GetObserver(), own);
            if(report.winner != EMPTY){
                assert(report.winner == ALLY || report.winner == ENEMY);
                r_result = EvaluationReport(report.m
                    , report.depth
                    , EvaluationReport::Mode::STATIC_TACTICS
                    , report.winner == ALLY ? EvaluationReport::Type::SUCCESS
                                            : EvaluationReport::Type::FAIL);
            }
        }
    };

    static void MakeDepthLimitReport(EvaluationReport& r_result, VariationManager& mgr){
        r_result = EvaluationReport(mgr.GetDepthController().DepthLimit()
                , EvaluationReport::Mode::DEEP_TACTICS
                , EvaluationReport::Type::DEPTH_LIMIT);
    }

    template<typename EvAgent>
    class BranchS4 : public BaseEvaluator{
    public:
        static constexpr TProperty SquareClass(){
            return TProperty::S4;
        }

        void operator()(EvaluationReport& r_result, VariationManager& mgr, Piece own) const override{
            assert(mgr.obs.SquareCount(own, TProperty::WIN) == 1);
            auto blocker = mgr.GetObserver().AnySquare(own, TProperty::WIN);

            auto mv = Move(blocker, OppositePiece(own));
            auto pos_ptr = mgr.FindPositionDataPtr(mv);
            if(!pos_ptr){
                auto v_answer = AutoVariation(mgr, mv);
                EvAgent()(r_result, mgr, own);
            }
            else{
                r_result = pos_ptr->GetReport();
            }

            if(r_result.Success()){
                r_result.moves.push_front(Move(blocker, OppositePiece(own)));
            }
        }
    };

    template<typename EvAgent>
    class Scan4SOnly : public BaseEvaluator{
    public:
        void operator()(EvaluationReport& r_result, VariationManager& mgr, Piece own) const override{
            if(!P_Final<EvAgent>()(r_result, mgr, own)){
                if(mgr.AtMaxDepth()){
                    if(!MoveGenerator(mgr.GetObserver(), own, TProperty::S4).Empty()){
                        MakeDepthLimitReport(r_result, mgr);
                    }
                }
                else{
                    auto s4gen = MoveGenerator(mgr.GetObserver(), own, TProperty::S4);
                    VariationScanner<BranchS4<Scan4SOnly<EvAgent>>>()(r_result, mgr, own, s4gen);
                }
            }
            if(r_result.Final()){
                auto pos_ptr = mgr.GetPositionDataPtr(own);
                pos_ptr->SetReport(r_result);
            }
        }
    };

    template<typename EvAgent>
    class BranchD3 : public BaseEvaluator{
    public:
        static constexpr TProperty SquareClass(){
            return TProperty::D3;
        }

        void operator()(EvaluationReport& r_result, VariationManager& mgr, Piece own) const override{
            assert(!mgr.obs.HaveSquares(own, TProperty::WIN));
            assert(mgr.obs.HaveSquares(own, TProperty::D4));
            EvaluationReport counter_report;
            Scan4SOnly<StaticTacticsAdapter>()(counter_report, mgr, OppositePiece(own));
            if(counter_report.Success()){
                return;
            }

            if(counter_report.DepthLimit()){
                r_result = std::move(counter_report);
                return;   //TODO: обрывать перебор при достижении макс. глубины (false)
            }

            assert(!counter_links.result.Fail());

            EvaluationReport report_candidate;
            MoveGenerator d4_blockers(mgr.GetObserver(), OppositePiece(own), TProperty::ANY);
            for(; d4_blockers.Valid(); d4_blockers.Next()){
                EvaluationReport next_report;
                auto pos_ptr = mgr.FindPositionDataPtr(d4_blockers.GetMove());
                if(!pos_ptr){
                    auto v_answer = AutoVariation(mgr, d4_blockers.GetMove());
                    EvAgent()(next_report, mgr, own);
                }
                else{
                    next_report = pos_ptr->GetReport();
                }

                if(!next_report.Success()){
                    if(next_report.DepthLimit()){
                        r_result = std::move(next_report);
                    }
                    return;
                }

                next_report.moves.push_front(d4_blockers.GetMove());
                if(!report_candidate.Final()
                   || next_report.FullDepth() > report_candidate.FullDepth()){
                    report_candidate = std::move(next_report);
                }
            }

            if(d4_blockers.Empty()){
                report_candidate = EvaluationReport(
                        4
                        , EvaluationReport::Mode::DEEP_TACTICS
                        , EvaluationReport::Type::SUCCESS);
            }

            std::vector<Point> targets;
            for(MoveGenerator gen(mgr.GetObserver(), OppositePiece(own), TProperty::S4);
                gen.Valid(); gen.Next()){
                targets.push_back(gen.Get());
            }
            for(auto& sq: targets){
                Move mv(sq, OppositePiece(own));
                EvaluationReport blocker_report;
                //TODO: придумать как вынести отдельной функцией
                auto v_start = AutoVariation(mgr, mv);
                StaticTacticsAdapter()(blocker_report, mgr, own);
                if(blocker_report.Success()){
                    continue;
                }
                auto blocker = mgr.GetObserver().AnySquare(OppositePiece(own), TProperty::WIN);
                auto v_answer = AutoVariation(mgr, Move(blocker, own));
                //ODOT:
                if(mgr.GetObserver().HaveSquares(own, TProperty::WIN)){
                    BranchS4<EvAgent>()(blocker_report, mgr, own);
                }
                else if(mgr.GetObserver().HaveSquares(own, TProperty::D4)){
                    BranchD3<EvAgent>()(blocker_report, mgr, own);
                }
                else{
                    return;
                }
                if(!blocker_report.Success()){
                    if(blocker_report.DepthLimit()){
                        r_result = std::move(blocker_report);
                    }
                    return;
                }
                else{
                    blocker_report.moves.push_front(v_answer);
                    blocker_report.moves.push_front(v_start);
                    if(blocker_report.FullDepth() > report_candidate.FullDepth()){
                        report_candidate = std::move(blocker_report);
                    }
                }
            }

            r_result = std::move(report_candidate);
        }
    };

    template<typename EvAgent>
    class Scan4S3D : public BaseEvaluator{
    public:
        void operator()(EvaluationReport& r_result, VariationManager& mgr, Piece own) const override{
            if(!P_Final<EvAgent>()(r_result, mgr, own)){
                if(mgr.AtMaxDepth()){
                    if(!MoveGenerator(mgr.GetObserver(), own, TProperty::S4).Empty()
                     || !MoveGenerator(mgr.GetObserver(), own, TProperty::D3).Empty()){
                        MakeDepthLimitReport(r_result, mgr);
                    }
                }
                else{
                    auto s4gen = MoveGenerator(mgr.GetObserver(), own, TProperty::S4);
                    VariationScanner<BranchS4<Scan4S3D<EvAgent>>>()(r_result, mgr, own, s4gen);
                    if(!r_result.Final()){
                        auto d3gen = MoveGenerator(mgr.GetObserver(), own, TProperty::D3);
                        VariationScanner<BranchD3<Scan4S3D<EvAgent>>>()(r_result, mgr, own, d3gen);
                    }
                }
            }
            auto pos_ptr = mgr.GetPositionDataPtr(own);
            pos_ptr->SetReport(r_result);
        }
    };
}

#endif //XO_BRANCH_H
