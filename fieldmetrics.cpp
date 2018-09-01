#include "fieldmetrics.h"
#include <cassert>


namespace XO{
    namespace Direction {
        const int8_t m_X[] = {1, 1, 0, -1};
        const int8_t m_Y[] = {0, 1, 1, 1};
    }

    Point FieldMetrics::MakePointSlow(DValueT id) const{
        assert(InBounds(id));
        return Point(id % m_width, id / m_width, id);
    }

    void FieldMetrics::Init(ValueT w, ValueT h)
    {
        m_width = w;
        m_height = h;
        m_square_count = w*h;
        for(int i = 0; i < 4; i++){
            m_offset[i] = Point(Direction::m_X[i], Direction::m_Y[i]
                                , Direction::m_X[i] + Direction::m_Y[i] * w);
            m_X4_offset[i] = m_offset[i];
            m_X4_offset[i].MultX4();
        }

        m_squares.clear();
        m_squares.resize(m_square_count);
        for(DValueT id = 0; id < m_square_count; id++){
            m_squares[id] = MakePointSlow(id);
        }

        m_middle = MakePoint(m_width >> 1, m_height >> 1);
    }

    Point FieldMetrics::MakePoint(ValueT x, ValueT y) const{
        assert(InBounds(x, y));
        return Point(x, y, x + y * m_width);
    }

    Point FieldMetrics::MakePoint(DValueT id) const{
        assert(InBounds(id));
        return m_squares[id];
    }
}
