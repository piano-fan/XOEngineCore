#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "tactics.h"
#include "tactics_deprecated.h"
#include "staticevaluation.h"
#include "baseevaluator.h"


namespace XO{
    class EvaluationManager : public BaseEvaluator{
    public:
        void operator()(EvaluationReport& r_result, VariationManager& mgr, Piece own) const override{
            mgr.Reset();

            if(mgr.GetObserver().GetMoveCount() == 0){
                r_result = EvaluationReport(Move(mgr.GetObserver().Metrics().Middle(), own)
                        , 1
                        , EvaluationReport::Mode::UNDEFINED
                        , EvaluationReport::Type::NONE);
                return;
            }

            if(Move mv; StaticTactics::SingleMove(mv, mgr.GetObserver(), own)){
                r_result = EvaluationReport(mv
                        , 1, EvaluationReport::Mode::UNDEFINED, EvaluationReport::Type::NONE);
                return;
            }

            static constexpr DepthT MAX_DEPTH_LIMIT = 18;
            for(auto depth = 2; depth <= MAX_DEPTH_LIMIT; depth += 2){
                r_result.Clear();
                mgr.SetDepthLimit(depth);
                if(P_Final<Tactics>()(r_result, mgr, own)){
                    return;
                }
                if(!r_result.DepthLimit()){
                    break;
                }
            }

            if(Move result; Tactics_Deprecated()(result, mgr.GetObserver(), own)){
                r_result = EvaluationReport(result
                        , 1
                        , EvaluationReport::Mode::DEPRECATED_TACTICS
                        , EvaluationReport::Type::SUCCESS);
                return;
            }

            {
                StaticEvaluator::Report report;
                StaticEvaluator()(report, mgr.GetObserver(), own);
                r_result = EvaluationReport(report.m
                        , 1
                        , EvaluationReport::Mode::STATIC_EVALUATION
                        , report.attacker == own ? EvaluationReport::Type::SUCCESS
                                                 : EvaluationReport::Type::FAIL);
                return;
            }
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
