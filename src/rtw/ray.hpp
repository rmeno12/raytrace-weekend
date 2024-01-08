#pragma once

#include "vec3.hpp"

class ray {
   public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    auto origin() const -> point3 { return orig; }
    auto direction() const -> vec3 { return dir; }

    auto at(float t) const -> point3 { return orig + t * dir; }

   private:
    point3 orig;
    vec3 dir;
};
