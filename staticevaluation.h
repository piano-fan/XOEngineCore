#ifndef XO_POSITIONSCORECACHE_H
#define XO_POSITIONSCORECACHE_H

#include "types.h"
#include "squareobserver.h"


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

            for(auto& sq: obs.GetSquares()){
                if(obs.GetPiece(sq) != EMPTY){
                    continue;
                }

                for(auto& p: Players){
                    SquareScore nextscore = obs.GetScore(sq, p);

                    if(bestvalue[p] < nextscore.Value()){
                        top_scored[p] = sq;
                        bestvalue[p] = nextscore.Value();
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
