#ifndef XO_BRANCH_H
#define XO_BRANCH_H

#include "variationmanager.h"
#include "variationscanner.h"
#include "statictactics.h"


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
                links.result = pos_ptr->data;
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
            auto pos_ptr = links.mgr.GetPositionDataPtr(own);
            pos_ptr->data = links.result;
        }
    };
}

#endif //XO_BRANCH_H
