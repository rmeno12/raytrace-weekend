#pragma once

#include <iostream>

#include "vec3.hpp"

using color = vec3;

void write_color(std::ostream &out, const color& pixel_color, int samples_per_pixel) {
    auto scale = 1.0 / samples_per_pixel;
    auto scale_color = scale * pixel_color;


    out << static_cast<int>(255.999 * scale_color.x()) << ' '
        << static_cast<int>(255.999 * scale_color.y()) << ' '
        << static_cast<int>(255.999 * scale_color.z()) << '\n';
}
