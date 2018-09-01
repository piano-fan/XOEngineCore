#ifndef XO_PIECESETCACHE_H
#define XO_PIECESETCACHE_H

#include <array>
#include "types.h"
#include "pieceset.h"
#include "gameobserver.h"
#include "fieldmetrics.h"


namespace XO{
    class PieceSetCache : public GameObserver{
        static std::vector<std::array<PieceSet8, 4>> m_backup;
        std::vector<std::array<PieceSet8, 4>> m_data;
        const FieldMetrics* m_metrics = nullptr;
    public:
        PieceSetCache();

        const std::array<PieceSet8, 4>& Get(Point t) const;
        PieceSet8 Get(Point t, ValueT dir) const;
        std::string ToString(Point t) const;

        void OnMove(Point t, Piece p) override;
        void OnTakeBack(Point t) override;
        void OnNewGame(const FieldMetrics* metrics) override;
    };
}

#endif // XO_PIECESETCACHE_H
