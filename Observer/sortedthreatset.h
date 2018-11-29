#ifndef XO_SORTEDTHREATSET_H
#define XO_SORTEDTHREATSET_H

#include "threat.h"
#include <array>


namespace XO{
    class SortedThreatSet{
        std::array<Threat, 4> m_threats;    //dir -> threat
        std::array<ValueT, 4> m_ranks;      //dir -> rank
        std::array<ValueT, 4> m_directions; //rank -> dir
    public:
        SortedThreatSet()
            :m_ranks{0, 1, 2, 3}, m_directions{0, 1, 2, 3}
            , m_threats{Threat(), Threat(), Threat(), Threat()}
        {}

        void Set(Threat t, ValueT dir){
            for(ValueT i = 0; i < 4; ++i){
                m_ranks[i] -= m_ranks[dir] < m_ranks[i];
            }
            m_threats[dir] = t;
            m_ranks[dir] = -1;
            for(ValueT i = 0; i < 4; ++i){
                m_ranks[dir] += m_threats[dir] <= m_threats[i];
            }
            for(ValueT i = 0; i < 4; ++i){
                m_ranks[i] += m_threats[dir] > m_threats[i];
            }
            for(ValueT i = 0; i < 4; ++i){
                m_directions[m_ranks[i]] = i;
            }
        }

        const Threat& operator[](ValueT sequence) const{
            return m_threats[m_directions[sequence]];
        }
        ValueT GetDirection(ValueT sequence) const{
            return m_directions[sequence];
        }
        ValueT GetRank(ValueT direction) const{
            return m_ranks[direction];
        }

        std::string ToString() const{
            std::string result = "";
            for(int i = 0; i < 4; ++i){
                result += "(" + std::to_string(GetDirection(i)) + ") " + operator[](i).ToString() + "\n";
            }
            return result;
        }
    };
}

#endif //XO_SORTEDTHREATSET_H
