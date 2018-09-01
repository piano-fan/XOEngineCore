#ifndef XO_FIELDMETRICS_H
#define XO_FIELDMETRICS_H

#include "types.h"


namespace XO{
    class FieldMetrics{
        std::vector<Point> m_squares;

        Point m_offset[4];
        Point m_X4_offset[4];
        Point m_middle;

        DValueT m_square_count;
        ValueT m_width;
        ValueT m_height;

        Point MakePointSlow(DValueT id) const;
    public:
        using const_iterator = std::vector<Point>::const_iterator;
        const_iterator begin() const{
            return m_squares.begin();
        }
        const_iterator end() const{
            return m_squares.end();
        }

        FieldMetrics()
                :m_width(0), m_height(0), m_square_count(0)
        {}

        void Init(ValueT w, ValueT h);

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

        Point GetOffset(int dir) const{
            return m_offset[dir];
        }
        Point GetX4Offset(int dir) const{
            return m_X4_offset[dir];
        }

        Point MakePoint(ValueT x, ValueT y) const;
        Point MakePoint(DValueT id) const;
        Point Middle() const{
            return m_middle;
        }
    };
}

#endif //XO_FIELDMETRICS_H
