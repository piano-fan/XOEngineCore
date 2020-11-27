#ifndef XO_FIELDMETRICS_H
#define XO_FIELDMETRICS_H

#include "types.h"
#include "math.h"


namespace XO{
    class FieldMetrics{
        Point m_middle;

        DValueT m_square_count;
        ValueT m_width;
        ValueT m_height;

    public:
        FieldMetrics()
                :m_width(0), m_height(0), m_square_count(0)
        {}

        void Init(ValueT w, ValueT h){
            Point::m_width = w; //TODO: fix

            m_width = w;
            m_height = h;
            m_square_count = w*h;

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

        Point Middle() const{
            return m_middle;
        }
    };
}

#endif //XO_FIELDMETRICS_H
