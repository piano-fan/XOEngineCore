#ifndef XO_FIELDMETRICS_H
#define XO_FIELDMETRICS_H

#include "types.h"
#include "math.h"


namespace XO{
    namespace Direction {
        enum{
            X = 0, XY = 1, Y = 2, YX = 3, COUNT = 4
        };
        constexpr std::array<OffsetT, 4> m_X = {1, 1, 0, -1};
        constexpr std::array<OffsetT, 4> m_Y = {0, 1, 1, 1};
    }

    class FieldMetrics{
        Point m_offset[4][2];
        Point m_middle;

        Point m_star_offset[32];

        DValueT m_square_count;
        ValueT m_width;
        ValueT m_height;

        Point GetOffset(int dir, int positive = 1) const{
            return m_offset[dir][positive];
        }
    public:
        FieldMetrics()
                :m_width(0), m_height(0), m_square_count(0)
        {}

        void Init(ValueT w, ValueT h){
            Point::m_width = w; //TODO: fix

            m_width = w;
            m_height = h;
            m_square_count = w*h;
            for(int i = 0; i < 4; i++){
                m_offset[i][1] = Point(Direction::m_X[i], Direction::m_Y[i]);
                m_offset[i][0] = -Point(Direction::m_X[i], Direction::m_Y[i]);
            }

            for(auto current = StarOffset::Begin(); current.Valid(); ++current){
                auto &ref = m_star_offset[current.ID()];
                ref = GetOffset(current.Angle(), current.Forward()) * (current.Distance() + 1);
            }

            m_middle = Point(m_width >> 1, m_height >> 1);
        }

        ValueT GetWidth() const{
            return m_width;
        }
        ValueT GetHeight() const{
            return m_height;
        }
        DValueT GetSquareCount() const{
            return m_square_count;
        }

        bool InBounds(DValueT id) const{
            return id < m_square_count;
        }
        bool InBounds(ValueT x, ValueT y) const{
            return x < m_width
                   && y < m_height;
        }
        bool InBounds(Point pos) const{
            return InBounds(pos.GetX(), pos.GetY());
        }

        Point MakePoint(Point center, StarOffset d) const{
            return center + m_star_offset[d.ID()];
        }

        Point Middle() const{
            return m_middle;
        }

        StarOffset MakeOffset(Point from, Point to) const{
            auto vec = to - from;
            assert(vec.GetX() != 0 || vec.GetY() != 0);
            OffsetT dir_x = sign(vec.GetX());
            OffsetT dir_y = sign(vec.GetY());
            OffsetT abs_x = abs(vec.GetX()) - 1;
            OffsetT abs_y = abs(vec.GetY()) - 1;

            if (abs_x >= 4 || abs_y >= 4) return StarOffset::End();

            StarOffset result = StarOffset::End();
            bool forward;
            if(vec.GetX() == 0){
                forward = (dir_y == Direction::m_Y[Direction::Y]);
                result = StarOffset(Direction::Y, forward, abs_y);
            }
            if(vec.GetY() == 0){
                forward = (dir_x == Direction::m_X[Direction::X]);
                result = StarOffset(Direction::X, forward, abs_x);
            }
            if(vec.GetX() == vec.GetY()){
                forward = (dir_x == Direction::m_X[Direction::XY]);
                result = StarOffset(Direction::XY, forward, abs_x);
            }
            if(vec.GetX() == -vec.GetY()){
                forward = (dir_x == Direction::m_X[Direction::YX]);
                result = StarOffset(Direction::YX, forward, abs_x);
            }
            return result;
        }
    };
}

#endif //XO_FIELDMETRICS_H
