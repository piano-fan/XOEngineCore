#include "types.h"
#include "core.h"
#include "threatcache.h"
#include "directioniterator.h"
#include <sstream>
#include <cassert>


namespace XO{
    Core::Core():
            m_square_cache(m_mask_cache), m_position_cache(*this)
    {}

    void Core::Init(){
        ThreatInfoCache::Init();

        Attach(&m_mask_cache);
        Attach(&m_square_cache);
        Attach(&m_position_cache);
    }

    void Core::NewGame(){
        m_state.NewGame();
        NotifyNewGame(nullptr);
    }

    void Core::Resize(unsigned int w, unsigned int h){
        if(m_state.GetMetrics().GetWidth() != w || m_state.GetMetrics().GetHeight() != h){
            m_state.Resize(w, h);
            NotifyNewGame(&m_state.GetMetrics());
        }
        else{
            m_state.NewGame();
            NotifyNewGame(nullptr);
        }
    }

    void Core::MakeMove(Point t){
        NotifyMove(t, m_state.GetTurn());
        m_state.MakeMove(t);
    }

    void Core::TakeBack(Point t){
        NotifyTakeBack(t);
        m_state.TakeBack(t);
    }

    bool Core::IsWin() const{ //не влияет на производительность ядра и скорость расчета одного хода
        /*return !m_state.GetMovelist().empty() //Быстрее но зависит от реализации ядра
               && m_core->IsWin(m_state.GetMovelist().back(), OppositePiece(m_state.GetTurn()));*/

        if(m_state.GetMovelist().empty()){
            return false;
        }

        for(unsigned int dir = 0; dir < 4; dir++){
            auto iter = DirectionIterator(m_state.GetMovelist().back(), dir, &m_state.GetMetrics());
            Piece last_turn = OppositePiece(m_state.GetTurn());
            unsigned int counter = 0;
            do{
                if(!iter.InBounds() || m_state.GetField()[iter.Current()] != last_turn){
                    counter = 0;
                    continue;
                }
                if(++counter >= 4){
                    return true;
                }
            }while((++iter).Valid());
        }
        return false;
    }

    bool Core::IsOver() const{
        return !m_state.CanMove() || IsWin();
    }

    Point Core::BestMove(Piece turn) const{
        return m_position_cache.Get().GetDumbBestMove(m_state.GetTurn());
    };

    std::string Core::SquareToString(Point t) const{
        return m_mask_cache.ToString(t) + m_square_cache.Get(t, X).ToString() + '\n'
                                    + m_square_cache.Get(t, O).ToString() + '\n';
    }
}
