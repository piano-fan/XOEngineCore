#ifndef XO_SQUARESCORECACHE_H
#define XO_SQUARESCORECACHE_H

#include <array>
#include "types.h"
#include "gameobserver.h"
#include "squarescore.h"
#include "fieldmetrics.h"
#include "piecesetcache.h"


namespace XO{
    class SquareScoreCache : public GameObserver{
        mutable std::vector<std::array<SquareScore, 2>> m_data;
        static std::vector<std::array<SquareScore, 2>> m_backup;
        const FieldMetrics* m_metrics;
        const PieceSetCache* m_masks;
        void Invalidate(Point t);
    public:
        SquareScoreCache();
        SquareScoreCache(const PieceSetCache& masks);

        const SquareScore& Get(Point t, Piece p) const;

        void OnMove(Point t, Piece p) override;
        void OnTakeBack(Point t) override;
        void OnNewGame(const FieldMetrics* metrics) override;
    };
}

#endif // XO_SQUARESCORECACHE_H
