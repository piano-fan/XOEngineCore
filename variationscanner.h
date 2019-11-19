#ifndef XO_VARIATIONSCANNER_H
#define XO_VARIATIONSCANNER_H

#include "variationmanager.h"
#include "movegenerator.h"


namespace XO{
    template<typename TesterT>
    class VariationTester{
    public:
        void operator()(EvaluationReport& r_result, VariationManager& mgr
                        , Piece own, Point sq) const{
            auto variation = AutoVariation(mgr, Move(sq, own), TesterT::SquareClass());
            EvaluationReport next_result;
            TesterT()(next_result, mgr, own);
            if(next_result.Success()){
                r_result = std::move(next_result);
                r_result.moves.push_front(variation);  //Обобщить??
            }
            if(!r_result.DepthLimit() && next_result.DepthLimit()){
                r_result = std::move(next_result);
                r_result.moves = {variation};
            }
        }
    };

    template<typename TesterT>
    class VariationScanner{
    public:
        void operator()(EvaluationReport& r_result, VariationManager& mgr
                        , Piece own, MoveGenerator& gen) const{
            std::list<Point> targets[4];
            for(; gen.Valid(); gen.Next()){
                auto tier = mgr.GetObserver().GetThreats(gen.GetMove())[1].GetTier();
                targets[tier].push_back(gen.Get());
            }
            for(auto& builder_tier: {3, 2, 1, 0})
            for(auto& sq: targets[builder_tier]){
                VariationTester<TesterT>()(r_result, mgr, own, sq);
                if(r_result.Success()){
                    return;
                }
            }
        }
    };
}


#endif //XO_VARIATIONSCANNER_H
