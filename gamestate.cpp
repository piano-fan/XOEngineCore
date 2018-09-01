#include "gamestate.h"


namespace XO{

    void GameState::NewGame() {
        m_field.NewGame();
        m_movelist.clear();
        m_turn = X;
    }

    void GameState::Resize(unsigned int w, unsigned int h) {
        m_metrics.Init(w, h);
        m_field.Resize(&m_metrics);
        m_movelist.clear();
        m_turn = X;
    }

    void GameState::MakeMove(Point t) {
        m_movelist.push_back(t);
        m_field[t] = m_turn;
        m_turn = OppositePiece(m_turn);
    }

    void GameState::TakeBack(Point t) {
        m_movelist.pop_back();
        m_field[t] = EMPTY;
        m_turn = OppositePiece(m_turn);
    }

    bool GameState::CanMove() const {
        return m_movelist.size() < m_metrics.GetSquareCount();
    }
}