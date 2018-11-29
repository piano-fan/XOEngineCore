#ifndef XO_OFFSET_2D_H
#define XO_OFFSET_2D_H

#include "builtintypes.h"


namespace XO{
    union Offset1D{
        static constexpr const ValueT TOTAL_OFFSETS = 8;
        struct Fields{
            ValueT m_distance : 2;
            ValueT m_direction : 1;
            ValueT m_invalid : 1;
            ValueT m_filler : 4;

            constexpr Fields(ValueT distance, ValueT direction, ValueT not_valid)
                :m_distance(distance), m_direction(direction)
                , m_invalid(not_valid), m_filler(0)
            {}
        };

        Fields m_fields;
        ValueT m_id;

        constexpr Offset1D()
            :m_fields(0, 0, 1)
        {}

    public:
        static constexpr Offset1D End(){
            return Offset1D();
        }

        static constexpr Offset1D Begin(){
            return Offset1D(0, 0);
        }

        constexpr Offset1D(ValueT direction, ValueT distance)
                :m_fields(distance, direction, 0)
        {}

        constexpr ValueT Direction() const{
            return m_fields.m_direction;
        }
        constexpr ValueT Distance() const{
            return m_fields.m_distance;
        }
        constexpr bool Valid() const{
            return !m_fields.m_invalid;
        }
        constexpr void operator++(){
            assert(m_id < TOTAL_OFFSETS);
            ++m_id;
        }
    };
}

#endif //XO_OFFSET_2D_H
