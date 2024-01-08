#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
   public:
    sphere(point3 _center, float _radius) : center(_center), radius(_radius) {}

    auto hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& rec) const -> bool override {
        auto oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        // using discriminant to find out if roots exist
        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = std::sqrt(discriminant);

        // find the root in the range if it exists
        auto root = (-half_b - sqrtd) / a;
        if (root <= ray_tmin || root >= ray_tmax) {
            // try the other root if the first one is out of range
            root = (-half_b + sqrtd) / a;
            if (root <= ray_tmin || root >= ray_tmax) return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius;

        return true;
    }

   private:
    point3 center;
    float radius;
};
