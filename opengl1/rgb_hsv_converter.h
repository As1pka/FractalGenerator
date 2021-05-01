#pragma once
#include <math.h>

typedef struct {
    float r;       // a fraction between 0 and 1
    float g;       // a fraction between 0 and 1
    float b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    float h;       // angle in degrees
    float s;       // a fraction between 0 and 1
    float v;       // a fraction between 0 and 1
} hsv;

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


static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in);
rgb hsv2rgb(hsv in);
rgb rgb_conv(float i);