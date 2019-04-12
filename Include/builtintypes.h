#ifndef XO_BUILTINTYPES_H
#define XO_BUILTINTYPES_H

#include <cassert>

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <array>
#include <memory>
#include <map>


namespace XO {
    using ValueT = uint8_t;
    using DValueT = uint16_t;

    using DepthT = DValueT;

    using OffsetT = int8_t;
    using DOffsetT = int16_t;
}


#endif //XO_BUILTINTYPES_H
