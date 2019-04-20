#ifndef XO_BRANCH_H
#define XO_BRANCH_H

#include "variationmanager.h"
#include "variationscanner.h"
#include "statictactics.h"


namespace XO{
    template<typename EvAgent>
    class BranchS4 : public BaseEvaluator{
    public:
        bool operator()(BaseEvaluator::Data& links, Piece own) const override{
            auto blocker = links.obs.GetTrackedSquares(own, TProperty::WIN).front();
            auto v_answer = AutoVariation(links.mgr, Move(blocker, OppositePiece(own)));

            EvAgent()(links, own);

            if(links.result.Success()){
                links.result.moves.push_front(v_answer);
            }

            return links.result.Final();
        }
    };

    class S4Variation : public AutoVariation{
    public:
        S4Variation(VariationManager& mgr, const Move& m)
            :AutoVariation(mgr, m)
        {}

        static constexpr TProperty SquareClass(){
            return TProperty::S4;
        }

        std::string ToString() const override{
            return std::string("S4") + AbstractVariation::ToString();
        }
    };

    template<typename EvAgent>
    class Scan4SOnly : public BaseEvaluator{
    public:
        bool operator()(Data& links, Piece own) const override{
            EvAgent()(links, own)
                || VariationScanner<S4Variation, BranchS4<Scan4SOnly<EvAgent>>>()(links, own);
            return links.result.Final();
        }
    };
}

#endif //XO_BRANCH_H
