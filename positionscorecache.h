#ifndef XO_POSITIONSCORECACHE_H
#define XO_POSITIONSCORECACHE_H

#include "positionscore.h"
#include "gameobserver.h"


namespace XO{
    class Core;

    class PositionScoreCache : public GameObserver{
        mutable PositionScore m_pscore;
        Core* m_core;
    public:
        PositionScoreCache();
        PositionScoreCache(Core& core);
        const PositionScore& Get() const;

        void OnMove(Point t, Piece p) override;
        void OnTakeBack(Point t) override;
        void OnNewGame(const FieldMetrics* metrics) override;
    };
}

#endif //XO_POSITIONSCORECACHE_H
