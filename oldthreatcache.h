#ifndef XO_OLDTHREATCACHE_H
#define XO_OLDTHREATCACHE_H

#include <memory>
#include <array>
#include "pieceset.h"
#include "threatinfo.h"


namespace XO{
    namespace Old{
        const int THREAT_STRENGTH_COUNT = 2;

        enum ThreatStrength{
            SINGLE_THREAT = 0,
            DUAL_THREAT = 1,
        };


        class ThreatInfo_1_0{
            friend class ThreatCache_1_0;
            std::array<Threat, THREAT_STRENGTH_COUNT> m_data;
            Threat& GetRef(ThreatStrength s);
        public:
            ThreatInfo_1_0();
            const Threat& GetCRef(ThreatStrength s) const;
        };

        class ThreatCache_1_0{
            std::unique_ptr<std::array<ThreatInfo_1_0, TOTAL_MASK_COUNT>> m_data;

            bool Calculated(PieceSet8 mask, ThreatStrength s) const;
            std::function<bool(PieceSet8, Piece)> MakeTierTester(ThreatTier t, ThreatStrength s) const;
        public:
            ThreatCache_1_0(Piece p);
            ThreatInfo_1_0& operator[](PieceSet8 mask);
            const ThreatInfo_1_0& operator[](PieceSet8 mask) const;
        };
    }
}

#endif //XO_OLDTHREATCACHE_H
