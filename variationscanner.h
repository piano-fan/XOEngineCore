#ifndef XO_VARIATIONSCANNER_H
#define XO_VARIATIONSCANNER_H

#include "baseevaluator.h"
#include "movegenerator.h"


namespace XO{
    template<typename VariationT, typename TesterT>
    class VariationScanner : public BaseEvaluator{
        static void MakeDepthLimitReport(Data& links, Piece own){
            new(&links.result) EvaluationReport(links.mgr.GetDepthController().DepthLimit()
                    , EvaluationReport::Mode::DEEP_TACTICS
                    , EvaluationReport::Type::DEPTH_LIMIT);
        }
    public:
        bool operator()(Data& links, Piece own) const override{
            TrackedMoveGenerator<VariationT::SquareClass()> generator(links.mgr, own);
            if(links.mgr.AtMaxDepth()){
                if(!generator.Empty()){
                    MakeDepthLimitReport(links, own);
                }
                return false;
            }
            for(auto& builder_tier: {3, 2, 1, 0})   //TODO: fix 0 нужен не во всех алг-мах
            for(generator.Reset(); generator.Valid(); generator.Next()){
                if(links.obs.GetThreats(generator.Get())[1].GetTier() != builder_tier){
                    continue;
                }
                auto variation = VariationT(links.mgr, generator.Get());
                BaseEvaluator::Data next_links(links);
                TesterT()(next_links, own);
                if(next_links.result.Success()){
                    links.result = std::move(next_links.result);
                    links.result.moves.push_front(variation);  //Обобщить??
                    return true;
                }
                if(next_links.result.DepthLimit()){
                    links.result = std::move(next_links.result);
                }
            }
            return false;
        }
    };
}


#endif //XO_VARIATIONSCANNER_H
