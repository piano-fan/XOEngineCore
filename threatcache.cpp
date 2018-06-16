#include "threatinfo.h"


namespace XO{
    bool ThreatCache::Calculated(PieceMask8 mask, Piece p, ThreatStrength s) const{
        return (*this)[mask].GetCRef(p, s).GetTier() != NO_THREAT;
    }

    std::function<bool(PieceMask8, Piece)> ThreatCache::MakeTierTester(ThreatTier t, ThreatStrength s) const{
        return [this, t, s](PieceMask8 mask, Piece p){
            return (*this)[mask].GetCRef(p, s).GetTier() == t;
        };
    }

    std::function<bool(PieceMask8, Piece)> ThreatCache::MakeBlockerTester(ThreatTier t, ThreatStrength s) const{
        return [this, t, s](PieceMask8 mask, Piece p){
            return (*this)[mask].GetCRef(OppositePiece(p), s).GetTier() < t;
        };
    }

    ThreatCache::ThreatCache():
        m_data(std::make_unique<std::array<ThreatInfoSet, TOTAL_MASK_COUNT>>())
    {
        auto mask = PieceMask8();

        for(Piece &piece: std::array<Piece, 2>{X, O}) {
            do {
                auto result = mask.IsWin(piece) ? 1 : 0;
                for (ThreatStrength &str: std::array<ThreatStrength, 2>{SINGLE_THREAT, DUAL_THREAT}) {
                    if (result) {
                        (*this)[mask].GetRef(piece, str).Set(WIN, 1);
                    } else {
                        (*this)[mask].GetRef(piece, str).Set(NO_THREAT, 0); //Инициализация
                    }
                }
            } while (++mask);
        }

        std::function<bool(PieceMask8, Piece)> tester = MakeTierTester(WIN, SINGLE_THREAT);
        for(Piece &piece: std::array<Piece, 2>{X, O}) {
            do {
                if (Calculated(mask, piece, SINGLE_THREAT)) {
                    continue;
                }
                auto result = mask.TestNullMoves(piece, tester);
                if (!result.none()) {
                    (*this)[mask].GetRef(piece, SINGLE_THREAT).Set(WIN_THREAT, result.count());
                    if (result.count() >= 2) {
                        (*this)[mask].GetRef(piece, DUAL_THREAT).Set(WIN_THREAT, 1);
                    }
                }
            } while (++mask);
        }

        for(Piece &piece: std::array<Piece, 2>{X, O}) {
            for (ThreatTier &tier: std::array<ThreatTier, 3>{PRE_WIN_THREAT, HAVE_OPEN_ALLY, CAN_WIN}) {
                for (ThreatStrength &str: std::array<ThreatStrength, 2>{SINGLE_THREAT, DUAL_THREAT}) {
                    tester = MakeTierTester(static_cast<ThreatTier>(tier + 1), str);
                    do {
                        if (Calculated(mask, piece, str)) {
                            continue;
                        }
                        auto result = mask.TestNullMoves(piece, tester);
                        if (!result.none()) {
                            (*this)[mask].GetRef(piece, str).Set(tier, result.count());
                        }
                    } while (++mask);
                }
            }
        }
    }


    const ThreatCache& ThreatCache::Instance(){
        static ThreatCache cache;
        return cache;
    }

    ThreatInfoSet& ThreatCache::operator[](PieceMask8 mask){
        return (*m_data)[mask.GetID()];
    }

    const ThreatInfoSet& ThreatCache::operator[](PieceMask8 mask) const{
        return (*m_data)[mask.GetID()];
    }
}
