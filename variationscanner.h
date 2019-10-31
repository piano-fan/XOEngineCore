#ifndef XO_VARIATIONSCANNER_H
#define XO_VARIATIONSCANNER_H

#include "baseevaluator.h"
#include "movegenerator.h"


namespace XO{
    template<typename TesterT>
    class VariationTester{
    public:
        void operator()(BaseEvaluator::Data& links, Piece own, Point sq) const{
            auto variation = AutoVariation(links.mgr, Move(sq, own), TesterT::SquareClass());
            BaseEvaluator::Data next_links(links);
            TesterT()(next_links, own);
            if(next_links.result.Success()){
                links.result = std::move(next_links.result);
                links.result.moves.push_front(variation);  //Обобщить??
            }
            if(!links.result.DepthLimit() && next_links.result.DepthLimit()){
                links.result = std::move(next_links.result);
                links.result.moves = {variation};
            }
        }
    };

    template<typename TesterT>
    class VariationScanner{
    public:
        void operator()(BaseEvaluator::Data& links, Piece own, MoveGenerator& gen) const{
            std::list<Point> targets[4];
            for(; gen.Valid(); gen.Next()){
                auto tier = links.obs.GetThreats(gen.GetMove())[1].GetTier();
                targets[tier].push_back(gen.Get());
            }
            for(auto& builder_tier: {3, 2, 1, 0})
            for(auto& sq: targets[builder_tier]){
                VariationTester<TesterT>()(links, own, sq);
                if(links.result.Success()){
                    return;
                }
            }
        }
    };
}


#endif //XO_VARIATIONSCANNER_H
