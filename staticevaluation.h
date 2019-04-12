#ifndef XO_POSITIONSCORECACHE_H
#define XO_POSITIONSCORECACHE_H

#include "types.h"
#include "baseevaluator.h"
#include "squarescore.h"


namespace XO{
    class StaticEvaluator : public BaseEvaluator{
    public:
        bool operator()(Data& links, Piece own) const override{
            Piece opp = OppositePiece(own);

            unsigned int bestvalue[2] = {0, 0};
            Point top_scored[2];

            for(auto& sq_class: {TProperty::D3, TProperty::S4}){
                for(auto& p: Players){
                    for(auto& sq: links.obs.GetTrackedSquares(p, sq_class)){
                        SquareScore nextscore;
                        nextscore.Calculate(links.obs.GetInfluence(sq), p);

                        if(bestvalue[p] < nextscore.Value()){
                            top_scored[p] = sq;
                            bestvalue[p] = nextscore.Value();
                        }
                    }
                }
            }

            if(!bestvalue[ALLY] && !bestvalue[ENEMY]){
                for(auto& sq: links.obs.GetSquares()){
                    if(links.obs.GetPiece(sq) != EMPTY){
                        continue;
                    }

                    for(auto& p: Players) {
                        SquareScore nextscore;
                        nextscore.Calculate(links.obs.GetInfluence(sq), p);

                        if(bestvalue[p] < nextscore.Value()){
                            top_scored[p] = sq;
                            bestvalue[p] = nextscore.Value();
                        }
                    }
                }
            }

            Piece leader;
            if(bestvalue[own] >= bestvalue[opp]){
                leader = own;
            }
            else{
                leader = opp;
            }

            new(&links.result) EvaluationReport(Move(top_scored[leader], own)
                    , 1
                    , EvaluationReport::Mode::STATIC_EVALUATION
                    , leader == own ? EvaluationReport::Type::SUCCESS
                                    : EvaluationReport::Type::FAIL);

            return true;
        }
    };
}

#endif //XO_POSITIONSCORECACHE_H
