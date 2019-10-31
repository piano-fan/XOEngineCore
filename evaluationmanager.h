#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "statictactics.h"
#include "tactics_deprecated.h"
#include "staticevaluation.h"
#include "baseevaluator.h"


namespace XO{
    class EvaluationManager : public BaseEvaluator{
    public:
        void operator()(Data& links, Piece own) const override{
            if(links.obs.GetMoveCount() == 0){
                links.result = EvaluationReport(Move(links.obs.Metrics().Middle(), own)
                        , 1
                        , EvaluationReport::Mode::UNDEFINED
                        , EvaluationReport::Type::NONE);
                return;
            }

            if(Move mv; StaticTactics::SingleMove(mv, links.obs, own)){
                links.result = EvaluationReport(mv
                        , 1, EvaluationReport::Mode::UNDEFINED, EvaluationReport::Type::NONE);
                return;
            }

            {
                if(P_Final<StaticTactics>()(links, own)){
                    return;
                }
            }

            if(Move result; Tactics_Deprecated()(result, links.obs, own)){
                links.result = EvaluationReport(result
                        , 1
                        , EvaluationReport::Mode::DEPRECATED_TACTICS
                        , EvaluationReport::Type::SUCCESS);
                return;
            }

            {
                StaticEvaluator::Report report;
                StaticEvaluator()(report, links.obs, own);
                links.result = EvaluationReport(report.m
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
