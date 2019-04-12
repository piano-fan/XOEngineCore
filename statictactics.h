#ifndef XO_STATICTACTICS_H
#define XO_STATICTACTICS_H

#include "squareobserver.h"


namespace XO{
    class StaticTactics{
    public:
        static bool Blocks(const SquareObserver& obs, const Move& m,
                           StarOffset target, TacticSet to_block){
            assert(target.Valid());
            auto target_point = obs.Metrics().MakePoint(m.GetPos(), target);
            assert(obs.ValidMove(target_point));
            assert(obs.GetTactics(Move(target_point, OppositePiece(m.GetTurn()))).AnyOf(to_block));
            auto influence = obs.GetInfluence(target_point);
            if(influence.GetThreats(OppositePiece(m.GetTurn())).GetRank(target.Angle()) > 1){
                return false;
            }
            influence.OnNeighbourChanged(m.GetTurn(), target);
            return !influence.GetTactics(OppositePiece(m.GetTurn())).AnyOf(to_block);
        }

        static bool Blocks(const SquareObserver& obs, const Move& m,
                            Point target, TacticSet to_block){
            if(m.GetPos() == target){
                return true;
            }

            auto offset = obs.Metrics().MakeOffset(m.GetPos(), target);
            if(!offset.Valid()){
                return false;
            }

            return Blocks(obs, m, offset, to_block);
        }

        static bool FullBlocker(const SquareObserver &obs, const Move &m
                                , TProperty sq_class, TacticSet to_block){
            for(auto& sq: obs.GetTrackedSquares(OppositePiece(m.GetTurn()), sq_class)){
                if(!Blocks(obs, m, sq, to_block)){
                    return false;
                }
            }
            return true;
        }

        static bool FullBlocker(const SquareObserver &obs, const Move &m, TProperty sq_class){
            return FullBlocker(obs, m, sq_class, sq_class);
        }

        static bool InstantWin(const SquareObserver& obs, Piece own){
            return obs.HaveSquares(own, TProperty::WIN);
        }

        static bool InstantLoss(const SquareObserver& obs, Piece own){
            return !InstantWin(obs, own)
                && obs.SquareCount(OppositePiece(own), TProperty::WIN) >= 2;
        }

        static bool SingleMove(Move& r_move, const SquareObserver& obs, Piece own){
            auto result = !InstantWin(obs, own)
                && obs.SquareCount(OppositePiece(own), TProperty::WIN) == 1;
            if(result){
                r_move = Move(obs.AnySquare(OppositePiece(own), TProperty::WIN), own);
            }
            return result;
        }

        struct Report{
            Move m;
            Piece winner = EMPTY;
            DepthT depth;
        };

        void operator()(Report& r_result, const SquareObserver& obs, Piece own) const{
            if(InstantWin(obs, own)){
                r_result = Report{
                    Move(obs.AnySquare(own, TProperty::WIN), own)
                    , ALLY
                    , 1
                };
                return;
            }

            if(InstantLoss(obs, own)){
                r_result = Report{
                    Move(obs.AnySquare(OppositePiece(own), TProperty::WIN), own)
                    , ENEMY
                    , 2
                };
                return;
            }

            if(Move mv; SingleMove(mv, obs, own)){
                auto& own_tactics = obs.GetTactics(mv);
                if(own_tactics.GetProperty(TProperty::D4)){
                    r_result = Report{
                        mv
                        , ALLY
                        , 3
                    };
                    return;
                }

                if(own_tactics.GetProperty(TProperty::D3_D3)
                 && !obs.HaveSquares(OppositePiece(own), TProperty::D4)
                 && !obs.HaveSquares(OppositePiece(own), TProperty::S4)){
                    r_result = Report{
                        mv
                        , ALLY
                        , 5
                    };
                    return;
                }

                return;
            }

            //Simple fork check
            if(obs.HaveSquares(own, TProperty::D4)){
                r_result = Report{
                    Move(obs.AnySquare(own, TProperty::D4), own)
                    , ALLY
                    , 3
                };
                return;
            }

            if(obs.HaveSquares(own, TProperty::D3_D3)
             && !obs.HaveSquares(OppositePiece(own), TProperty::D4)
             && !obs.HaveSquares(OppositePiece(own), TProperty::S4)){
                r_result = Report{
                    Move(obs.AnySquare(own, TProperty::D3_D3), own)
                    , ALLY
                    , 5
                };
                return;
            }
        }
    };
}

#endif //XO_STATICTACTICS_H
