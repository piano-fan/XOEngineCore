#include "oldthreatcache.h"


namespace XO{
    namespace Old{
        ThreatInfo_1_0::ThreatInfo_1_0(){
            m_data.fill(Threat());
        }

        Threat& ThreatInfo_1_0::GetRef(ThreatStrength s){
            return m_data[s];
        }

        const Threat& ThreatInfo_1_0::GetCRef(ThreatStrength s) const{
            return m_data[s];
        }


        bool ThreatCache_1_0::Calculated(PieceSet8 mask, ThreatStrength s) const{
            return (*this)[mask].GetCRef(s).GetTier() != ThreatTier::NO_THREAT;
        }

        std::function<bool(PieceSet8, Piece)> ThreatCache_1_0::MakeTierTester(ThreatTier t, ThreatStrength s) const{
            return [this, t, s](PieceSet8 mask, Piece p){
                return (*this)[mask].GetCRef(s).GetTier() == t;
            };
        }

        ThreatCache_1_0::ThreatCache_1_0(Piece piece):
                m_data(std::make_unique<std::array<ThreatInfo_1_0, TOTAL_MASK_COUNT>>())
        {
            auto mask = PieceSet8();

            do {
                auto result = mask.IsWin(piece) ? 1 : 0;
                for (ThreatStrength &str: std::array<ThreatStrength, 2>{SINGLE_THREAT, DUAL_THREAT}) {
                    if (result && str == SINGLE_THREAT) {
                        (*this)[mask].GetRef(str).SetTier(ThreatTier::WIN);
                    } else {
                        (*this)[mask].GetRef(str).SetTier(ThreatTier::NO_THREAT); //Инициализация
                    }
                }
            } while (++mask);

            std::function<bool(PieceSet8, Piece)> tester = MakeTierTester(ThreatTier::WIN, SINGLE_THREAT);
            do {
                if (Calculated(mask, SINGLE_THREAT)) {
                    continue;
                }
                auto result = mask.TestNullMoves(piece, tester);
                if (!result.none()) {
                    auto &singleref = (*this)[mask].GetRef(SINGLE_THREAT);
                    singleref.SetTier(ThreatTier::WIN_THREAT);
                    singleref.SetScore(result.count());
                    if (result.count() >= 2) {
                        auto &dualref = (*this)[mask].GetRef(DUAL_THREAT);
                        dualref.SetTier(ThreatTier::WIN_THREAT);
                        dualref.SetScore(1);
                    }
                }
            } while (++mask);

            for (ThreatTier &tier: std::array<ThreatTier, 3>{ThreatTier::PRE_WIN_THREAT, ThreatTier::HAVE_ALLY, ThreatTier::CAN_WIN}) {
                for (ThreatStrength &str: std::array<ThreatStrength, 2>{SINGLE_THREAT, DUAL_THREAT}) {
                    tester = MakeTierTester(static_cast<ThreatTier>(static_cast<ValueT>(tier) + 1), str);
                    do {
                        if (Calculated(mask, str)) {
                            continue;
                        }
                        auto result = mask.TestNullMoves(piece, tester);
                        if (!result.none()) {
                            auto &ref = (*this)[mask].GetRef(str);
                            ref.SetTier(tier);
                            ref.SetScore(result.count());
                        }
                    } while (++mask);
                }
            }
        }

        ThreatInfo_1_0& ThreatCache_1_0::operator[](PieceSet8 mask){
            return (*m_data)[mask.GetID()];
        }

        const ThreatInfo_1_0& ThreatCache_1_0::operator[](PieceSet8 mask) const{
            return (*m_data)[mask.GetID()];
        }

    }
}