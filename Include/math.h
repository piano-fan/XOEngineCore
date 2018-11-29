#ifndef XO_MATH_H
#define XO_MATH_H

#include <cmath>


namespace XO{
    template <typename T> int sign(T v){
        return (T(0) < v) - (v < T(0));
    }
}

#endif //XO_MATH_H
