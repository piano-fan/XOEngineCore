#ifndef XO_SQUARESCORE_H
#define XO_SQUARESCORE_H

#include "squareinfluence.h"


namespace XO {
    enum class PositionalSquareTier : ValueT {
        NONE = 0,
        CAN_SINGLE = 1,
        CAN_DUAL = 2,
        CONNECTED = 3,
        SINGLE_THREAT = 4,
        DUAL_THREAT = 5,
        CONNECTED_N_SINGLE_THREAT = 6,
        CONNECTED_N_DUAL_THREAT = 7,
        FORCED = 8,
    };

    class SquareScore{
        PositionalSquareTier m_positional_tier;
        int m_score;
        int m_value;
	public:
        void Calculate(const SquareInfluence &infl, Piece p);

        int Value() const{
            return m_value;
        }

        std::string ToString() const{
            return std::string("P:") + std::to_string(static_cast<int>(m_positional_tier))
                   + " Sc:" + std::to_string(static_cast<int>(m_score))
                   + " Val:" + std::to_string(Value());
        }
    };
}

#endif //XO_SQUARESCORE_H
