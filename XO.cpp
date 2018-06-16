#include "XO.h"
#include <cassert>
#include <algorithm>
#include <ctime>


namespace XO {
    void Init(){
        ThreatCache::Instance();
    }

    Game::Game(unsigned int w, unsigned int h):
        m_field(std::make_shared<Field>(w, h)), m_movelist(w*h), m_move(X), m_winner(EMPTY), m_movecount(0)
    {}

    bool Game::GameOver(){
        return (Winner() != EMPTY) || !CanMove();
    }

    bool Game::XMove(){
        return m_move == X;
    }

    Piece Game::Move(){
        return m_move;
    }

    void Game::SwapMove(){
        m_move = (m_move == X) ? O : X;
    }

    Piece Game::Winner(){
        return m_winner;
    }

    int Game::GetMoveCount(){
        return m_movecount;
    }

    bool Game::CanMove(){
        return GetMoveCount() < m_field->GetSquareCount();
    }

    const Game::MoveListT& Game::GetMoveList() const{
        return m_movelist;
    }

    void Game::MakeMove(int x, int y){
        assert(Winner() == EMPTY);
        m_field->GetSquare(x, y).PlacePiece(Move());
        if(m_field->GetSquare(x, y).IsWin(Move())){
            m_winner = Move();
        }
        SwapMove();
        m_movelist[m_movecount].first = x;
        m_movelist[m_movecount].second = y;
        m_movecount++;
    }

    void Game::MakeMove(const FieldIterator& pos){
        MakeMove(pos.GetX(), pos.GetY());
    }

    void Game::TakeBack(){
        assert(m_movecount >= 0);
        m_winner = EMPTY;
        m_movecount--;
        int x = m_movelist[m_movecount].first;
        int y = m_movelist[m_movecount].second;
        m_field->GetSquare(x, y).ClearPiece();
        SwapMove();
    }

    std::string Game::SquareToString(int x, int y){
        std::string result = std::to_string(x) + "/" + std::to_string(y) + "\n";
        result += m_field->GetSquare(x, y).ToString();
        return result;
    }

    int Game::BestThreat(FieldIterator &r_best, bool own){
        Piece p = own ? Move() : OppositePiece(Move());
        r_best = FieldIterator(m_field.get());
        Piece first = r_best.SeekEmpty().GetPiece();
        assert(first != OUT_OF_BOUNDS);
        int bestvalue = r_best.GetValue(p);

        auto next = r_best;
        while(1){
            if((++next).GetPiece() == OUT_OF_BOUNDS){   //TODO: remove
                break;
            }
            if(next.SeekEmpty().GetPiece() == OUT_OF_BOUNDS){
                break;
            }
            int nextvalue = next.GetValue(p);
            if(bestvalue < nextvalue){
                r_best = next;
                bestvalue = nextvalue;
            }
        }
        return bestvalue;
    }

    FieldIterator Game::DumbBestMove(){
        if(m_movecount == 0) {
            return m_field->Middle();
        }

        FieldIterator owniter, oppiter;
        int own = BestThreat(owniter);
        int opp = BestThreat(oppiter, false);

        if(own >= opp){
            return owniter;
        }
        else{
            return oppiter;
        }
    };
}
