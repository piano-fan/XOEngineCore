#ifndef XO_SQUARESCORE_H
#define XO_SQUARESCORE_H

#include "squareinfluence.h"


namespace XO {
    union SquareScore{
    private:
        struct Fields{
            ValueT m_secondary_score;
            ValueT m_primary_score;
            ValueT m_secondary_tier : 4;
            ValueT m_primary_tier : 4;
            ValueT m_tactical;
        };

        Fields m_fields;
        unsigned int m_value;

	public:
        void Calculate(const SquareInfluence &infl, Piece p);

        unsigned int Value() const{
            return m_value;
        }

        std::string ToString() const{
            return std::string("Tactics: ") + std::to_string((int)m_fields.m_tactical)
                   + " 1st T:" + std::to_string((int)m_fields.m_primary_tier)
                   + " Sc:" + std::to_string((int)m_fields.m_primary_score)
                   + " 2nd T:" + std::to_string((int)m_fields.m_secondary_tier)
                   + " Sc:" + std::to_string((int)m_fields.m_secondary_score)
                   + " Val:" + std::to_string(Value());
        }
    };
}

#endif //XO_SQUARESCORE_H
