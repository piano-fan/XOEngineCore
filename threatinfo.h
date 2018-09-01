#ifndef XO_THREATINFO_H
#define XO_THREATINFO_H

#include "types.h"


namespace XO{
    enum class ThreatTier : ValueT {
        NO_THREAT = 0,
        CAN_WIN = 1,
        HAVE_ALLY = 2,
        PRE_WIN_THREAT = 3,
        WIN_THREAT = 4,
        WIN = 5,
    };

    class Threat{
        ThreatTier m_tier : 3;
        ValueT m_dual : 1;
        ValueT m_score : 4;
    public:
        Threat():
                m_tier(ThreatTier::NO_THREAT), m_score(0), m_dual(0)
        {}

        void SetTier(ThreatTier tier){
            m_tier = tier;
        }
        void SetDual(ValueT value){
            m_dual = value;
        }
        void SetScore(ValueT score){
            m_score = score;
        }

        ThreatTier GetTier() const{
            return m_tier;
        }
        ValueT GetDual() const{
            return m_dual;
        }
        ValueT GetScore() const{
            return m_score;
        }



        std::string ToString() const{
            return std::string("Tier:") + std::to_string(static_cast<int>(m_tier)) + " Dual tier:" + std::to_string(static_cast<int>(m_dual))
                   + " Score:" + std::to_string(static_cast<int>(m_score));
        }
    };
}


#endif //XO_THREATINFO_H
