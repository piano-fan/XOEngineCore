#ifndef XO_THREATINFO_H
#define XO_THREATINFO_H

#include "types.h"


namespace XO{
    union Threat{
    private:
        struct Fields{
            ValueT m_score : 3;
            ValueT m_sd_combo : 1;  //TODO: improve?
            ValueT m_dual : 1;
            ValueT m_tier : 3;

            Fields(){}
            constexpr Fields(ValueT score, ValueT sd_combo, ValueT dual, ValueT tier)
                    :m_score(score), m_dual(dual), m_tier(tier), m_sd_combo(sd_combo)
            {}
        };

        Fields m_fields;
        ValueT m_value;
    public:
        constexpr Threat()
                :m_fields(0, 0, 0, 0)
        {}

        constexpr Threat(ValueT tier, ValueT is_dual, ValueT sd_combo = 0, ValueT score = 0)
                :m_fields{score, sd_combo, is_dual, tier}
        {}

        constexpr void SetTier(ValueT tier){
            m_fields.m_tier = tier;
        }
        constexpr void SetDual(ValueT value){
            m_fields.m_dual = value;
        }
        constexpr void SetSDCombo(ValueT sd_combo){
            m_fields.m_sd_combo = sd_combo;
        }
        constexpr void SetScore(ValueT score){
            m_fields.m_score = score;
        }

        constexpr ValueT GetTier() const{
            return m_fields.m_tier;
        }
        constexpr ValueT GetFullTier() const{
            return (m_fields.m_tier << 1) + m_fields.m_dual;
        };
        constexpr ValueT GetMixedTier() const{
            return m_fields.m_tier + m_fields.m_dual;
        }
        constexpr ValueT GetDual() const{
            return m_fields.m_dual;
        }
        constexpr ValueT GetSDCombo() const{
            return m_fields.m_sd_combo;
        }
        constexpr ValueT GetScore() const{
            return m_fields.m_score;
        }

        constexpr ValueT GetValue() const{
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
