#ifndef XO_OLDTHREATCACHE_H
#define XO_OLDTHREATCACHE_H

#include <array>
#include <bitset>
#include "pieceset.h"


namespace XO{
    namespace Old{
        const int THREAT_STRENGTH_COUNT = 2;

        class Threat_1_0{
            ValueT m_score : 4;
            ValueT m_tier : 3;

        public:
            constexpr Threat_1_0(ValueT tier = 0, ValueT score = 0)
                    :m_tier(tier), m_score(score)
            {}

            constexpr void SetTier(ValueT tier){
                m_tier = tier;
            }
            constexpr void SetScore(ValueT score){
                m_score = score;
            }

            constexpr ValueT GetTier() const{
                return m_tier;
            }
            constexpr ValueT GetScore() const{
                return m_score;
            }

            constexpr bool IsNone() const{
                return GetTier() == 0;
            }
        };

        class ThreatInfo_1_0{
            friend class ThreatCache_1_0;
            std::array<Threat_1_0, THREAT_STRENGTH_COUNT> m_data;
            constexpr Threat_1_0& GetRef(ValueT is_dual){
                return m_data[is_dual];
            }
        public:
            constexpr ThreatInfo_1_0()
                :m_data{Threat_1_0(), Threat_1_0()}
            {}

            constexpr const Threat_1_0& GetCRef(ValueT is_dual) const{
                return m_data[is_dual];
            }
        };

        class ThreatCache_1_0{
            std::array<ThreatInfo_1_0, PieceSet8::TOTAL_COUNT> m_data;

            constexpr bool Calculated(PieceSet8 mask, ValueT is_dual) const{
                return !(*this)[mask].GetCRef(is_dual).IsNone();
            }

            constexpr std::bitset<8> TestNullMoves(const PieceSet8& set, Piece piece
                    , Threat_1_0 t, ValueT is_dual){
                std::bitset<8> result(0);
                for(auto offset = Offset1D::Begin(); offset.Valid(); ++offset){
                    Piece next = set.GetPiece(offset);
                    if (next == EMPTY) {
                        PieceSet8 test = set;
                        test.SetPiece(piece, offset);
                        if((*this)[test].GetCRef(is_dual).GetTier() == t.GetTier()){
                            result.set(offset.Distance() + (offset.Direction() << 2));    //TODO: return result not tested
                        }
                    }
                }
                return result;
            }

        public:
            constexpr ThreatCache_1_0(Piece p){
                auto mask = PieceSet8();

                do {
                    auto result = mask.IsWin(p) ? 1 : 0;
                    for (auto& is_dual: {0, 1}) {
                        if (result && !is_dual) {
                            (*this)[mask].GetRef(is_dual).SetTier(5);
                        } else {
                            (*this)[mask].GetRef(is_dual).SetTier(0); //Инициализация
                        }
                    }
                } while (++mask);

                do {
                    if (Calculated(mask, false)) {
                        continue;
                    }
                    auto result = TestNullMoves(mask, p, Threat_1_0(5), 0);
                    if (!result.none()) {
                        auto &singleref = (*this)[mask].GetRef(false);
                        singleref.SetTier(4);
                        singleref.SetScore(mask.SolidBlockLength(p));
                        if (result.count() >= 2) {
                            auto &dualref = (*this)[mask].GetRef(true);
                            dualref.SetTier(4);
                            dualref.SetScore(1);
                        }
                    }
                } while (++mask);

                for (auto& tier_value: {3, 2, 1}) {
                    for (auto& is_dual: {0, 1}) {
                        do {
                            if (Calculated(mask, is_dual)) {
                                continue;
                            }
                            auto result = TestNullMoves(mask, p
                                    , Threat_1_0(tier_value + 1), is_dual);
                            if (!result.none()) {
                                auto &ref = (*this)[mask].GetRef(is_dual);
                                ref.SetTier(tier_value);
                                ref.SetScore(result.count());   //TODO: improve
                            }
                        } while (++mask);
                    }
                }
            }
            constexpr ThreatInfo_1_0& operator[](PieceSet8 mask){
                return m_data[mask.GetID()];
            }
            constexpr const ThreatInfo_1_0& operator[](PieceSet8 mask) const{
                return m_data[mask.GetID()];
            }
        };
    }
}

#endif //XO_OLDTHREATCACHE_H
