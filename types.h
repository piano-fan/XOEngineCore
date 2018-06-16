#ifndef XO_TYPES_H
#define XO_TYPES_H

#include <cstdint>


namespace XO{
    enum ThreatTier{
        WIN = 5,
        WIN_THREAT = 4,
        PRE_WIN_THREAT = 3,
        HAVE_OPEN_ALLY = 2,
        CAN_WIN = 1,
        NO_THREAT = 0,
    };

    enum ThreatStrength{
        SINGLE_THREAT = 0,
        DUAL_THREAT = 1,
    };

    using ValueT = uint8_t;
}

#endif //XO_TYPES_H
