#ifndef XO_POINT_H
#define XO_POINT_H

#include "builtintypes.h"


namespace XO{
    class Point{
        OffsetT m_x;
        OffsetT m_y;
        DOffsetT m_i;
    public:
        Point():
                m_x(0), m_y(0), m_i(0)
        {}
        Point(int x, int y, int id):
                m_x(x), m_y(y), m_i(id)
        {}

        int GetX() const{
            return static_cast<int>(m_x);
        }
        int GetY() const{
            return static_cast<int>(m_y);
        }
        int GetID() const{
            return static_cast<int>(m_i);
        }

        bool operator==(const Point& other) const{
            return GetID() == other.GetID();
        }

        Point operator-() const{
            return Point(-m_x, -m_y, -m_i);
        }
        Point operator-(const Point& b) const{
            return Point(m_x - b.m_x, m_y - b.m_y, m_i - b.m_i);
        }
        Point operator+(const Point& b) const{
            return Point(m_x + b.m_x, m_y + b.m_y, m_i + b.m_i);
        }

        Point& operator+=(Point offset){
            m_x += offset.m_x;
            m_y += offset.m_y;
            m_i += offset.m_i;
            return *this;
        }

        Point& operator-=(Point offset){
            m_x -= offset.m_x;
            m_y -= offset.m_y;
            m_i -= offset.m_i;
            return *this;
        }

        std::string ToString(char sep = ' ') const{
            return std::to_string(GetX()) + sep + std::to_string(GetY());
        }
    };
}

#endif //XO_POINT_H
