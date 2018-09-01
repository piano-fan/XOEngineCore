#ifndef XO_DIRECTIONITERATOR_H
#define XO_DIRECTIONITERATOR_H

#include "types.h"
#include "fieldmetrics.h"


namespace XO{
    class DirectionIterator{
        const FieldMetrics *m_metrics;
        Point m_current;
        Point m_offset;
        ValueT m_current_id;
        bool m_in_bounds;
    public:
        DirectionIterator(Point middle, ValueT dir, const FieldMetrics* metrics):
                m_current_id(0), m_metrics(metrics), m_offset(metrics->GetOffset(dir))
        {
            m_current = middle;
            m_current -= m_metrics->GetX4Offset(dir);
            m_in_bounds = m_metrics->InBounds(m_current);
        }

        DirectionIterator& operator++(){
            ++m_current_id;
            m_current += m_offset;
            if(m_current_id == 4){
                m_current += m_offset;
            }
            m_in_bounds = m_metrics->InBounds(m_current);
            return *this;
        }

        Point Current() const{
            return m_current;
        }
        ValueT CurrentID() const{
            return m_current_id;
        }
        bool InBounds() const{
            return m_in_bounds;
        }
        bool Valid() const{
            return m_current_id < 8;
        }
    };
}

#endif // XO_DIRECTIONITERATOR_H
