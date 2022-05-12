#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include <fstream>

void write_color(std::fstream& out, vec3 pixel_color, int samples) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples;

    r *= scale;
    g *= scale;
    b *= scale;
    out << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif