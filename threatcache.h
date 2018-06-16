#ifndef XO_THREATCACHE_H
#define XO_THREATCACHE_H

#include <memory>
#include <array>
#include "piecemask.h"


namespace XO{
    class ThreatInfo;
    class ThreatInfoSet;

    const int TOTAL_MASK_COUNT = 65536;


    class ThreatCache{
        std::unique_ptr<std::array<ThreatInfoSet, TOTAL_MASK_COUNT>> m_data;
        ThreatCache();
        ThreatCache(ThreatCache const&) = delete;
        ThreatCache& operator= (ThreatCache const&) = delete;
        ThreatInfoSet& operator[](PieceMask8 mask);
        bool Calculated(PieceMask8 mask, Piece p, ThreatStrength s) const;
        std::function<bool(PieceMask8, Piece)> MakeTierTester(ThreatTier t, ThreatStrength s) const;
        std::function<bool(PieceMask8, Piece)> MakeBlockerTester(ThreatTier t, ThreatStrength s) const;
    public:
        static const ThreatCache& Instance();
        const ThreatInfoSet& operator[](PieceMask8 mask) const;
    };
}


#endif //XO_THREATCACHE_H
