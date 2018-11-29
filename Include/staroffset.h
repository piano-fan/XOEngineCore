#ifndef XO_VECTOR_H
#define XO_VECTOR_H

#include "builtintypes.h"
#include "offset_1d.h"


namespace XO{
    union StarOffset{
        static constexpr const ValueT TOTAL_OFFSETS = Offset1D::TOTAL_OFFSETS * 4;
    private:
        struct Fields{
            ValueT m_distance : 2;
            ValueT m_forward : 1;
            ValueT m_angle : 2;
            ValueT m_not_valid : 1;
            ValueT m_filler : 2;

            constexpr Fields(ValueT distance, ValueT forward, ValueT angle, ValueT not_valid)
                :m_distance(distance), m_forward(forward)
                , m_angle(angle), m_not_valid(not_valid), m_filler(0)
            {}
        };

        Fields m_fields;
        ValueT m_id;

        constexpr StarOffset()
            :m_fields(0, 0, 0, 1)
        {}

    public:
        static constexpr StarOffset End(){
            return StarOffset();
        }

        constexpr static StarOffset Begin(){
            return StarOffset(0, 0, 0);
        }

        constexpr StarOffset(ValueT dir, ValueT forward, ValueT id)
            :m_fields(id, forward, dir, 0)
        {}

        StarOffset(ValueT dir, Offset1D offset)
            :StarOffset(dir, offset.Direction(), offset.Distance())
        {}

        void operator++(){
            assert(m_id < TOTAL_OFFSETS);
            ++m_id;
        }

        ValueT ID() const{
            return m_id;
        }
        constexpr ValueT Forward() const{
            return m_fields.m_forward;
        }
        constexpr ValueT Angle() const{
            return m_fields.m_angle;
        }
        constexpr ValueT Distance() const{
            return m_fields.m_distance;
        }
        Offset1D Offset() const{
            return Offset1D(Forward(), Distance());
        }
        Offset1D MirroredOffset() const{
            return Offset1D(1 - Forward(), Distance());
        }
        bool Valid() const{
            return !m_fields.m_not_valid;
        }

        std::string ToString() const{
            return std::string("Ang: ") + std::to_string((int)Angle())
                   + (" Fw: ") + std::to_string((int)Forward())
                   + (" Dst: ") + std::to_string((int)Distance());
        }
    };
}

#endif //XO_VECTOR_H
