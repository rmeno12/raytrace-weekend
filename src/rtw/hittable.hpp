#pragma once

#include "interval.hpp"
#include "ray.hpp"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    float t;
    bool front_face;

    auto set_face_normal(const ray& r, const vec3& outward_normal) -> void {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
   public:
    virtual ~hittable() = default;

    virtual auto hit(const ray& r, interval ray_t, hit_record& rec) const -> bool = 0;
};
