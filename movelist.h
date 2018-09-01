#ifndef XO_MOVELIST_H
#define XO_MOVELIST_H

#include "types.h"


namespace XO{
    class MoveList : public std::vector<Point>{
    public:
        std::string ToString(char move_sep = ' ', char coord_sep = ' ') const{
            std::string result("");
            for(const Point& t: *this){
                result += t.ToString(coord_sep) + move_sep;
            }
            return result;
        }
    };
}

#endif // XO_MOVELIST_H
