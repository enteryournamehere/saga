#pragma once

#include "nu2api/numath/nufloat.h"

static inline void eduiRGBToHSV(f32 red, f32 green, f32 blue, f32 &hue, f32 &saturation, f32 &value) {
    f32 maximum = red > (green > blue ? green : blue) ? red : (green > blue ? green : blue);
    f32 minimum = red < (green < blue ? green : blue) ? red : (green < blue ? green : blue);
    value = maximum;
    saturation = maximum != 0.0f ? (maximum - minimum) / maximum : 0.0f;
    if (saturation != 0.0f) {
        f32 delta = maximum - minimum;
        if (red == maximum)
            hue = (green - blue) / delta;
        else if (green == maximum)
            hue = 2.0f + (blue - red) / delta;
        else if (blue == maximum)
            hue = 4.0f + (red - green) / delta;
        hue *= 60.0f;
        if (hue < 0.0f)
            hue += 360.0f;
    } else {
        hue = 0.0f;
    }
}

static inline void eduiHSVToRGB(f32 hue, f32 saturation, f32 value, f32 &red, f32 &green, f32 &blue) {
    if (saturation == 0.0f) {
        red = green = blue = value;
    } else {
        if (hue == 360.0f)
            hue = 0.0f;
        hue /= 60.0f;
        i32 sector = static_cast<i32>(NuFloor(hue));
        f32 fraction = hue - sector;
        f32 low = (1.0f - saturation) * value;
        f32 falling = (1.0f - saturation * fraction) * value;
        f32 rising = (1.0f - (1.0f - fraction) * saturation) * value;
        switch (sector) {
        case 0:
            red = value;
            green = rising;
            blue = low;
            break;
        case 1:
            red = falling;
            green = value;
            blue = low;
            break;
        case 2:
            red = low;
            green = value;
            blue = rising;
            break;
        case 3:
            red = low;
            green = falling;
            blue = value;
            break;
        case 4:
            red = rising;
            green = low;
            blue = value;
            break;
        case 5:
            red = value;
            green = low;
            blue = falling;
            break;
        }
    }
}
