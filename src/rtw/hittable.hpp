#pragma once

#include "ray.hpp"

struct hit_record {
    point3 p;
    vec3 normal;
    float t;
};

class hittable {
   public:
    virtual ~hittable() = default;

    virtual auto hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& rec) const
        -> bool = 0;
};
