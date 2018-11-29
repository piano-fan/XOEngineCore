#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "staticevaluation.h"


namespace XO{
    class EvaluationManager : public BaseEvaluator{
    public:
        bool operator()(Data& links, Piece own) const override{
            if(links.obs.GetMoveCount() == 0){
                new(&links.result) EvaluationReport(Move(links.obs.Metrics().Middle(), own)
                        , 1
                        , EvaluationReport::Mode::UNDEFINED
                        , EvaluationReport::Type::NONE);
                return true;
            }

            return StaticEvaluator()(links, own);
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
