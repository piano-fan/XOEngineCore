#ifndef XO_MOVE_H
#define XO_MOVE_H

#include "builtintypes.h"
#include "point.h"
#include "piece.h"

namespace XO{
    class Move{
        Point m_pos;
        Piece m_turn;
    public:
        constexpr Move()
            :m_pos(Point()), m_turn(Piece())
        {}

        constexpr Move(Point pos, Piece turn)
            :m_pos(pos), m_turn(turn)
        {}

        bool operator==(const Move& other) const{
            return other.m_pos == m_pos && other.m_turn == m_turn;
        }

        Point GetPos() const{
            return m_pos;
        }
        Piece GetTurn() const{
            return m_turn;
        }

        Move Opposite() const{
            return Move(m_pos, OppositePiece(m_turn));
        }

        std::string ToString() const{
            return std::string(PieceNameShort[m_turn]) + m_pos.ToString('-');
        }
    };
}

#endif //XO_MOVE_H
