#pragma once

#include <iostream>

#include "vec3.hpp"

using color = vec3;

auto linear_to_gamma(float linear_component) -> float { return std::sqrt(linear_component); }

auto write_color(std::ostream& out, const color& pixel_color, int samples_per_pixel) -> void {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    const auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    out << static_cast<int>(255.999 * r) << ' ' << static_cast<int>(255.999 * g) << ' '
        << static_cast<int>(255.999 * b) << '\n';
}
