#ifndef XO_POINT_H
#define XO_POINT_H

#include "builtintypes.h"
#include "geometry/point.hpp"

namespace XO{
    class Point : public Geometry::Point<OffsetT> {
    typedef Geometry::Point<OffsetT> Super;
    public:
        static ValueT m_width;  //TODO: fix

        constexpr Point():
            Super()
        {}
        constexpr Point(int x, int y):
            Super(x, y)
        {}
        constexpr Point(const Super& s):
            Super(s)
        {}

        DOffsetT GetID() const{
            return GetX() + GetY() * m_width;
        }

        std::string ToString(char sep = ' ') const{
            return std::to_string(GetX()) + sep + std::to_string(GetY());
        }
    };
}

#endif //XO_POINT_H
