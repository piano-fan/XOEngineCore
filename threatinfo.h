#ifndef XO_THREATINFO_H
#define XO_THREATINFO_H


#include <string>
#include <array>
#include "piecemask.h"


namespace XO{
    const int PLAYER_COUNT = 2;
    const int THREAT_TIER_COUNT = 6;
    const int THREAT_STRENGTH_COUNT = 2;
    const int THREAT_TOTAL_COUNT = THREAT_TIER_COUNT * THREAT_STRENGTH_COUNT;

    const std::string ThreatTierName[] = {
            "IsWin", "WinThreat", "PreWinThreat", "HaveOpenAlly", "CanWin", "NoThreat"
    };

    const std::string ThreatStrengthName[] = {
            "Single", "Dual"
    };


    enum ThreatTier{
        WIN = 0,
        WIN_THREAT = 1,
        PRE_WIN_THREAT = 2,
        HAVE_OPEN_ALLY = 3,
        CAN_WIN = 4,
        NO_THREAT = 5,
    };

    enum ThreatStrength{
        SINGLE_THREAT = 0,
        DUAL_THREAT = 1,
    };


    class ThreatInfo {
    public:
        using ValueT = int8_t;
    private:
        std::array<ValueT, THREAT_TOTAL_COUNT> m_ThreatScore;
        int TStoID(ThreatTier t, ThreatStrength s);
    public:
        static void Init();

        ThreatInfo();
        ThreatInfo(PieceMask8 m, Piece p);
        void Print();

        void SetScore(ThreatTier t, ThreatStrength s, ValueT score);
        ValueT GetScore(ThreatTier t, ThreatStrength s);
        ThreatTier TopTier(ThreatStrength s);

        void operator+=(const ThreatInfo& ti);
        void operator-=(const ThreatInfo& ti);
        ThreatInfo Normalize();
    };
}


#endif //XO_THREATINFO_H
