#include "gameobserver.h"


namespace XO{
    void GameObserverSubject::Attach(GameObserver* obs){
        m_observers.push_back(obs);
    }

    void GameObserverSubject::NotifyMove(Point t, Piece p) const{
        for(auto &ptr: m_observers){
            ptr->OnMove(t, p);
        }
    }

    void GameObserverSubject::NotifyTakeBack(Point t) const{
        for(auto &ptr: m_observers){
            ptr->OnTakeBack(t);
        }
    }

    void GameObserverSubject::NotifyNewGame(const FieldMetrics* metrics) const{
        for(auto &ptr: m_observers){
            ptr->OnNewGame(metrics);
        }
    }
}
