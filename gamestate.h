#ifndef XO_GAMESTATE_H
#define XO_GAMESTATE_H

#include "fieldmetrics.h"
#include "field.h"
#include "movelist.h"


namespace XO{
    class GameState {
        Field m_field;
        FieldMetrics m_metrics;
        MoveList m_movelist;
        Piece m_turn;
    public:
        const Field &GetField() const {
            return m_field;
        }

        const FieldMetrics &GetMetrics() const {
            return m_metrics;
        }

        const MoveList &GetMovelist() const {
            return m_movelist;
        }

        const Piece GetTurn() const {
            return m_turn;
        }

        void NewGame();
        void Resize(unsigned int w, unsigned int h);
        void MakeMove(Point t);
        void TakeBack(Point t);
        bool CanMove() const;
    };
}

#endif //XO_GAMESTATE_H
