#ifndef XO_TACTICALTIER_H
#define XO_TACTICALTIER_H

#include "threat.h"
#include "sortedthreatset.h"


namespace XO{
    enum class TProperty : ValueT{
        D4 = 0, D3_D3 = 1, FORK = 2,
        WIN = 3, S4 = 4, D3 = 5, TACTICAL = 6, COUNT = 7
    };

    const std::string TPropertyName[]{
            "D4", "D3+D3", "Fork",
            "Win", "S4", "D3", "Tactical"
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
        { //TODO: 3я угроза может влиять на тактические решения
            //например тройная четверка может быть блокирована только в одной клетке что отсекает перебор
            Set(TProperty::WIN, primary.IsWin());
            Set(TProperty::FORK,
                primary.GetFullTier() >= Threat(4, 1).GetFullTier()
             || secondary.GetFullTier() >= Threat(3, 1).GetFullTier());
            Set(TProperty::D4,
                primary.GetFullTier() == Threat(4, 1).GetFullTier()
             || (primary.GetTier() == 4
              && secondary.GetTier() == 4));
            Set(TProperty::S4,
                primary.GetTier() == 4);
            Set(TProperty::TACTICAL,
                primary.GetFullTier() >= Threat(3, 1).GetFullTier());
            Set(TProperty::D3_D3,
                primary.GetFullTier() == Threat(3, 1).GetFullTier()
             && secondary.GetFullTier() == Threat(3, 1).GetFullTier());
            Set(TProperty::D3,
                primary.GetFullTier() == Threat(3, 1).GetFullTier());
        }

        constexpr TacticSet(TProperty property)
            :m_fields(0)
        {
            Set(property, true);
        }

        explicit TacticSet(const SortedThreatSet& set)
                :   TacticSet(set[0], set[1])
        {}

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
