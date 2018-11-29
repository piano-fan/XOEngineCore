#ifndef XO_THREATINFO_H
#define XO_THREATINFO_H

#include "pieceset.h"


namespace XO{
    union Threat{
    private:
        struct Fields{
            uint8_t m_score : 3;
            uint8_t m_sd_combo : 1;  //TODO: improve?
            uint8_t m_dual : 1;
            uint8_t m_tier : 3;

            Fields(){}
            constexpr Fields(uint8_t score, uint8_t sd_combo, uint8_t dual, uint8_t tier)
                    :m_score(score), m_sd_combo(sd_combo), m_dual(dual), m_tier(tier)
            {}
        };

        Fields m_fields;
        uint8_t m_value;
    public:
        constexpr Threat()
                :m_fields(0, 0, 0, 0)
        {}

        constexpr Threat(uint8_t tier, uint8_t is_dual, uint8_t sd_combo = 0, uint8_t score = 0)
                :m_fields{score, sd_combo, is_dual, tier}
        {}

        static Threat Get(Piece p, PieceSet8 mask);

        constexpr void SetTier(uint8_t tier){
            m_fields.m_tier = tier;
        }
        constexpr void SetDual(uint8_t value){
            m_fields.m_dual = value;
        }
        constexpr void SetSDCombo(uint8_t sd_combo){
            m_fields.m_sd_combo = sd_combo;
        }
        constexpr void SetScore(uint8_t score){
            m_fields.m_score = score;
        }

        constexpr uint8_t GetTier() const{
            return m_fields.m_tier;
        }
        constexpr uint8_t GetFullTier() const{
            return (m_fields.m_tier << 1) + m_fields.m_dual;
        };
        constexpr uint8_t GetMixedTier() const{
            return m_fields.m_tier + m_fields.m_dual;
        }
        constexpr uint8_t GetDual() const{
            return m_fields.m_dual;
        }
        constexpr uint8_t GetSDCombo() const{
            return m_fields.m_sd_combo;
        }
        constexpr uint8_t GetScore() const{
            return m_fields.m_score;
        }

        constexpr uint8_t GetValue() const{
            return m_value;
        };

        constexpr bool operator>(const Threat& other) const{
            return GetValue() > other.GetValue();
        }

        constexpr bool operator>=(const Threat& other) const{
            return GetValue() >= other.GetValue();
        }

        constexpr bool operator<(const Threat& other) const{
            return GetValue() < other.GetValue();
        }

        constexpr bool operator<=(const Threat& other) const{
            return GetValue() <= other.GetValue();
        }

        constexpr bool operator==(const Threat& other) const{
            return GetValue() == other.GetValue();
        }

        constexpr bool IsWin() const{
            return GetTier() == 5;
        }

        constexpr bool IsNone() const{
            return GetTier() == 0;
        }

        std::string ToString() const{
            return std::string("Tier:") + std::to_string(static_cast<int>(m_fields.m_tier))
                   + " Dual tier:" + std::to_string(static_cast<int>(m_fields.m_dual))
                   + " S/d combo:" +std::to_string(static_cast<int>(m_fields.m_sd_combo))
                   + " Score:" + std::to_string(static_cast<int>(m_fields.m_score));
        }
    };
}


#endif //XO_THREATINFO_H
