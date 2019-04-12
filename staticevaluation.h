#ifndef XO_POSITIONSCORECACHE_H
#define XO_POSITIONSCORECACHE_H

#include "types.h"
#include "squareobserver.h"
#include "squarescore.h"


namespace XO{
    class StaticEvaluator{
    public:
        struct Report{
            Move m;
            Piece attacker;
        };

        void operator()(Report& r_result, const SquareObserver& obs, Piece turn) const{
            unsigned int bestvalue[2] = {0, 0};
            Point top_scored[2];

            for(auto& sq_class: {TProperty::D4, TProperty::D3, TProperty::S4}){
                for(auto& p: Players){
                    for(auto& sq: obs.GetTrackedSquares(p, sq_class)){
                        SquareScore nextscore;
                        nextscore.Calculate(obs.GetInfluence(sq), p);

                        if(bestvalue[p] < nextscore.Value()){
                            top_scored[p] = sq;
                            bestvalue[p] = nextscore.Value();
                        }
                    }
                }
            }

            if(!bestvalue[ALLY] && !bestvalue[ENEMY]){
                for(auto& sq: obs.GetSquares()){
                    if(obs.GetPiece(sq) != EMPTY){
                        continue;
                    }

                    for(auto& p: Players) {
                        SquareScore nextscore;
                        nextscore.Calculate(obs.GetInfluence(sq), p);

                        if(bestvalue[p] < nextscore.Value()){
                            top_scored[p] = sq;
                            bestvalue[p] = nextscore.Value();
                        }
                    }
                }
            }

            Piece leader;
            if(bestvalue[turn] >= bestvalue[OppositePiece(turn)]){
                leader = turn;
            }
            else{
                leader = OppositePiece(turn);
            }

            r_result = Report{
                Move(top_scored[leader], turn)
                , leader
            };
        }
    };
}

#endif //XO_POSITIONSCORECACHE_H
