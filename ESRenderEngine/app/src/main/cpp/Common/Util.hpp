//
// Created by we.kim on 2017-07-14.
//

#ifndef UTIL_H
#define UTIL_H

#include "common.hpp"

#define STR(s) #s
#define TOSTR(s) STR(s)

namespace Util
{
    static const float PI = 3.141592f;

    static inline bool IsEqual(real real_value_a, real real_value_b, real epsilon = EPSILON)
    {
        return (fabs(real_value_a - real_value_b) < epsilon) ? true : false;
    }

    static inline bool IsZero(real value, real epsilon = EPSILON)
    {
        return IsEqual(value, 0);
    }


}

#endif
