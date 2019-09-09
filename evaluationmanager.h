#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "tactics.h"
#include "tactics_deprecated.h"
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

            static constexpr DValueT DEPTH_LIMIT = 14;
            links.mgr.Reset();
            links.mgr.SetDepthLimit(DEPTH_LIMIT);

            return StaticTactics<true>()(links, own)
            || Tactics()(links, own)
            || Tactics_Deprecated::Calculate(links, own)
            || StaticEvaluator()(links, own);
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
