#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline auto degrees_to_radians(float degrees) -> float { return degrees * pi / 180.0; }

inline auto random_float() -> float {
    static std::uniform_real_distribution<float> dist(0.0, 1.0);
    static std::mt19937 gen;
    return dist(gen);
}

inline auto random_float(float min, float max) -> float {
    return min + (max - min) * random_float();
}
