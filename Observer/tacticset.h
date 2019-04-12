#ifndef XO_TACTICALTIER_H
#define XO_TACTICALTIER_H

#include "threat.h"
#include "sortedthreatset.h"


namespace XO{
    enum class TProperty : ValueT{
        WIN = 0, D4 = 1,
        S4 = 2, D3 = 3,
        FORK = 4,
        D3_D3 = 5,
        ANY = 6,
        COUNT = 7
    };

    const std::string TPropertyName[]{
            "Win", "D4",
            "S4", "D3",
            "Fork",
            "D3+D3",
            "Any"
    };

    union TacticSet{
    private:
        using BitSet = ValueT;

        BitSet m_fields;

        constexpr void Set(TProperty property, bool value){//WARNING! Cant remove property
            m_fields |= (BitSet(value) << static_cast<unsigned int>(property));
        }

        constexpr TacticSet(BitSet fields)
            :m_fields(fields)
        {}
    public:
        constexpr TacticSet()
            :m_fields(0)
        {}

        constexpr TacticSet(Threat primary, Threat secondary)
            :m_fields(0)
        {   //WARNING! 3rd threat is decisive in some cases
            Set(TProperty::WIN, primary.IsWin());
            Set(TProperty::D4,
                primary.GetFullTier() == Threat(4, 1).GetFullTier()
                || (primary.GetTier() == 4
                    && secondary.GetTier() == 4));
            Set(TProperty::S4,
                primary.GetFullTier() == Threat(4, 0).GetFullTier()
             && secondary.GetTier() < 4);
            Set(TProperty::D3,
                primary.GetFullTier() == Threat(3, 1).GetFullTier());
            Set(TProperty::FORK,
                primary.GetFullTier() >= Threat(4, 1).GetFullTier()
             || secondary.GetFullTier() >= Threat(3, 1).GetFullTier());
            Set(TProperty::D3_D3,
                primary.GetFullTier() == Threat(3, 1).GetFullTier()
             && secondary.GetFullTier() == Threat(3, 1).GetFullTier());
            Set(TProperty::ANY, true);
        }

        constexpr TacticSet(TProperty property)
            :m_fields(0)
        {
            Set(property, true);
        }

        explicit TacticSet(const SortedThreatSet& set)
                :   TacticSet(set[0], set[1])
        {}

        ValueT Value() const{
            return (static_cast<ValueT>(GetProperty(TProperty::WIN)) << 4)
                    + (static_cast<ValueT>(GetProperty(TProperty::D4)) << 3)
                    + (static_cast<ValueT>(GetProperty(TProperty::FORK)) << 2)
                    + (static_cast<ValueT>(GetProperty(TProperty::S4)) << 1)
                    + static_cast<ValueT>(GetProperty(TProperty::D3));
        }

        constexpr bool GetProperty(TProperty property) const{
            return AnyOf(property);
        }

        TacticSet Promotions(TacticSet before) const{
            return TacticSet((before.m_fields ^ m_fields) & m_fields);
        }

        constexpr bool AnyOf(TacticSet other) const{
            return !(*this & other).Empty();
        }

        constexpr bool AllOf(TacticSet other) const{
            return (*this & other) == other;
        }

        constexpr TacticSet operator|(TacticSet other) const{
            return TacticSet(m_fields | other.m_fields);
        }

        constexpr TacticSet operator&(TacticSet other) const{
            return TacticSet(m_fields & other.m_fields);
        }

        constexpr bool operator==(TacticSet other) const{
            return m_fields == other.m_fields;
        }

        constexpr bool Empty() const{
            return m_fields == 0;
        }

        std::string ToString() const{
            std::string result = "";
            for(ValueT property = 0; property < (int)TProperty::COUNT; ++property){
                result += std::string(TPropertyName[property]) + ": "
                        + std::to_string(GetProperty(TProperty(property))) + " ";
            }
            return result;
        }
    };

    constexpr TacticSet operator|(TProperty a, TProperty b){
        return TacticSet(a) | TacticSet(b);
    }

    constexpr TacticSet operator&(TProperty a, TProperty b){
        return TacticSet(a) & TacticSet(b);
    }
}

#endif //XO_TACTICALTIER_H
