#ifndef XO_POINT_H
#define XO_POINT_H

#include "builtintypes.h"
#include "geometry/point.hpp"
#include "math.h"
#include "staroffset.h"

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

        bool IsStarOffset() const {
            if (GetX() > 4 || GetY() > 4
            || (GetX() != 0 && GetY() != 0 && abs(GetX()) != abs(GetY()))) {
                return false;
            }
            return true;
        }

        constexpr Point GetDirection() const {
            return Point(sign(GetX()), sign(GetY()));
        }

        constexpr OffsetT GetStarIndex() const {
            if (GetX()) return abs(GetX() - 1);
            return abs(GetY() - 1);
        }

        static constexpr Point GetDirection(const StarOffset& offset) {
            constexpr Point dir[2][4] {
                { Point(0, 1),  Point(-1, 1), Point(-1, 0), Point(-1, -1) },
                { Point(0, -1), Point(1, -1), Point(1, 0),  Point (1, 1)  }
            };
            return dir[offset.Forward()][offset.Angle()];
        }

        constexpr StarOffset GetOffset() const {
            constexpr StarOffset l_offset[3][3] = {
                { StarOffset(3, 0, 0), StarOffset(0, 1, 0),    StarOffset(1, 1, 0)},
                { StarOffset(2, 0, 0), StarOffset::End(), StarOffset(2, 1, 0)},
                { StarOffset(1, 0, 0), StarOffset(0, 0, 0),    StarOffset(3, 1, 0)}
            };
            return l_offset[GetY() + 1][GetX() + 1];
        }

        static constexpr StarOffset MakeOffset(const Point& diff) {
            auto step = diff.GetDirection().GetOffset();
            return StarOffset(step.Angle(), step.Forward(), diff.GetStarIndex());
        }

        Point Move(StarOffset offset) const {
            return *this + GetDirection(offset) * (offset.Distance() + 1);
        }

        std::string ToString(char sep = ' ') const{
            return std::to_string(GetX()) + sep + std::to_string(GetY());
        }
    };
}

#endif //XO_POINT_H
