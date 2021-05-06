#pragma once
#include "rgb_hsv_converter.h"


inline bool operator== (const rgb& c1, const rgb& c2)
{
    return (int(c1.r) == int(c2.r) &&
        int(c1.g) == int(c2.g) &&
        int(c1.b) == int(c2.b));
}

inline bool operator!= (const rgb& c1, const rgb& c2)
{
    return !(c1 == c2);
}

bool isCloseColor(const rgb color1, const rgb color2, float diff) {
    float curr_diff = abs(color1.r - color2.r) + abs(color1.g - color2.g) + abs(color1.b - color2.b);
    return curr_diff < diff;
}