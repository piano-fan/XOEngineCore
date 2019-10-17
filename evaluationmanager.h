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

            links.mgr.Reset();

            if(StaticTactics<true>()(links, own)){
                return true;
            }

            static constexpr DValueT MAX_DEPTH_LIMIT = 20;
            for(auto depth = 2; depth <= MAX_DEPTH_LIMIT; depth += 2){
                links.result.Clear();
                links.mgr.SetDepthLimit(depth);
                Tactics()(links, own);
                if(links.result.Final()){
                    return true;
                }
                if(!links.result.DepthLimit()){
                    break;
                }
            }

            for(auto depth = 2; depth <= MAX_DEPTH_LIMIT; depth += 2){
                links.result.Clear();
                links.mgr.SetDepthLimit(depth);
                Tactics_Deprecated::Calculate(links, own);
                if(links.result.Final()){
                    return true;
                }
                if(!links.result.DepthLimit()){
                    break;
                }
            }
            if(links.result.DepthLimit()){
                return true;
            }

            return StaticEvaluator()(links, own);
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
