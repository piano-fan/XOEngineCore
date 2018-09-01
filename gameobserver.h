#ifndef XO_GAMEOBSERVER_H
#define XO_GAMEOBSERVER_H

#include "types.h"


namespace XO{
    class FieldMetrics;

    class GameObserver{
    public:
        virtual void OnMove(Point t, Piece p) = 0;
        virtual void OnTakeBack(Point t) = 0;
        virtual void OnNewGame(const FieldMetrics* metrics) = 0;
    };

    class GameObserverSubject{
        std::vector<GameObserver*> m_observers;
    public:
        void Attach(GameObserver* obs);
        //void Detach(GameObserver* obs);

        void NotifyMove(Point t, Piece p) const;
        void NotifyTakeBack(Point t) const;
        void NotifyNewGame(const FieldMetrics* metrics) const;
    };
}

#endif // XO_GAMEOBSERVER_H
