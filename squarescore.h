#ifndef XO_SQUARESCORE_H
#define XO_SQUARESCORE_H

#include <array>
#include "types.h"


namespace XO {
    class PieceSet8;

    enum class TacticalSquareTier : ValueT {
        NONE = 0,
        NOT_TACTICAL = 1,
        LOWER_DUAL = 2,
        SINGLE = 3,
        DOUBLE_LOWER_DUAL = 4,
        SINGLE_N_LOWER_DUAL = 5,
        UPPER_DUAL = 6,
        WIN = 7,
    };

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
        ValueT m_score;
        ValueT m_valid = false;
        TacticalSquareTier m_tactical_tier;
        PositionalSquareTier m_positional_tier;
        int m_value;
    public:
        void Calculate(Piece p, const std::array<PieceSet8, 4> &masks);

        void ConsumeMask(Piece p, PieceSet8 m);

        bool Useless() const;
        bool Tactical() const;
        bool Forced() const;
        bool Win() const;

        int Value() const{
            return m_value;
        }
        bool Valid() const{
            return m_valid;
        }
        void Invalidate(){
            m_valid = false;
        }

        TacticalSquareTier GetTacticalTier() const;
        PositionalSquareTier GetPositionalTier() const;

        std::string ToString() const;
    };
}

#endif //XO_SQUARESCORE_H
