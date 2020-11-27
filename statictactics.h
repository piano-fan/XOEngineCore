#ifndef XO_STATICTACTICS_H
#define XO_STATICTACTICS_H

#include "squareobserver.h"
#include "report.h"


namespace XO{
    class StaticTactics{
    public:
        static bool Blocks(const SquareObserver& obs, const Move& m,
                           StarOffset target, TacticSet to_block){
            assert(target.Valid());
            auto target_point = m.GetPos().Move(target);
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

            Point diff = target - m.GetPos();
            if (!diff.IsStarOffset()) return false;

            return Blocks(obs, m, Point::MakeOffset(diff), to_block);
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

        static bool InstantWin(const SquareObserver& obs, Piece turn){
            return obs.HaveSquares(turn, TProperty::WIN);
        }

        static bool InstantLoss(const SquareObserver& obs, Piece turn){
            return !InstantWin(obs, turn)
                && obs.SquareCount(OppositePiece(turn), TProperty::WIN) >= 2;
        }

        static bool SingleMove(Move& r_move, const SquareObserver& obs, Piece turn){
            auto result = !InstantWin(obs, turn)
                && obs.SquareCount(OppositePiece(turn), TProperty::WIN) == 1;
            if(result){
                r_move = Move(obs.AnySquare(OppositePiece(turn), TProperty::WIN), turn);
            }
            return result;
        }

        void operator()(Report& r_result, const SquareObserver& obs, Piece turn) const{
            if(InstantWin(obs, turn)){
                r_result = Report{
                    Report::Author::STATIC_TACTICS
                    , Report::Type::SUCCESS
                    , Move(obs.AnySquare(turn, TProperty::WIN), turn)
                    , 1
                };
                return;
            }

            if(InstantLoss(obs, turn)){
                r_result = Report{
                    Report::Author::STATIC_TACTICS
                    , Report::Type::FAIL
                    , Move(obs.AnySquare(OppositePiece(turn), TProperty::WIN), turn)
                    , 2
                };
                return;
            }

            if(Move mv; SingleMove(mv, obs, turn)){
                auto& own_tactics = obs.GetTactics(mv);
                if(own_tactics.GetProperty(TProperty::D4)){
                    r_result = Report{
                        Report::Author::STATIC_TACTICS
                        , Report::Type::SUCCESS
                        , mv
                        , 3
                    };
                    return;
                }

                if(own_tactics.GetProperty(TProperty::D3_D3)
                 && !obs.HaveSquares(OppositePiece(turn), TProperty::D4)
                 && !obs.HaveSquares(OppositePiece(turn), TProperty::S4)){
                    r_result = Report{
                        Report::Author::STATIC_TACTICS
                        , Report::Type::SUCCESS
                        , mv
                        , 5
                    };
                    return;
                }
            }
            else{
                if(obs.HaveSquares(turn, TProperty::D4)){
                    r_result = Report{
                        Report::Author::STATIC_TACTICS
                        , Report::Type::SUCCESS
                        , Move(obs.AnySquare(turn, TProperty::D4), turn)
                        , 3
                    };
                    return;
                }

                if(obs.HaveSquares(turn, TProperty::D3_D3)
                 && !obs.HaveSquares(OppositePiece(turn), TProperty::D4)
                 && !obs.HaveSquares(OppositePiece(turn), TProperty::S4)){
                    r_result = Report{
                        Report::Author::STATIC_TACTICS
                        , Report::Type::SUCCESS
                        , Move(obs.AnySquare(turn, TProperty::D3_D3), turn)
                        , 5
                    };
                    return;
                }
            }
            r_result = Report{
                Report::Author::STATIC_TACTICS
                , Report::Type::NONE
                , Move()
                , 0
            };
        }
    };
}

#endif //XO_STATICTACTICS_H
