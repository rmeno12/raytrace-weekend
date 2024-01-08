#pragma once

#include <cmath>
#include <limits>
#include <memory>

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline auto degrees_to_radians(float degrees) -> float {
    return degrees * pi / 180.0;
}
