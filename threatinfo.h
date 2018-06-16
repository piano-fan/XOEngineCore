#ifndef XO_THREATINFO_H
#define XO_THREATINFO_H


#include <string>
#include <array>
#include "threatcache.h"


namespace XO{
    const int THREAT_STRENGTH_COUNT = 2;
    const int PLAYER_COUNT = 2;


    constexpr const char* ThreatStrengthName[] = {
            "Single", "Dual"
    };


    class ThreatInfo{
        ValueT m_tier : 3;
        ValueT m_score : 4;
    public:
        ThreatInfo();
        void Print() const;

        void Set(ThreatTier t, ValueT score);
        void SetTier(ThreatTier tier);
        void SetScore(ValueT score);
        ThreatTier GetTier() const;
        ValueT GetScore() const;
    };


    class Square;

    class ThreatInfoSet{
        friend class ThreatCache;
        std::array<ThreatInfo, THREAT_STRENGTH_COUNT * PLAYER_COUNT> m_data;
        ThreatInfo& GetRef(Piece p, ThreatStrength s);
    public:
        ThreatInfoSet();
        const ThreatInfo& GetCRef(Piece p, ThreatStrength s) const;
        void Print(Piece p, ThreatStrength s) const;
        void Print() const;
    };
}


#endif //XO_THREATINFO_H
