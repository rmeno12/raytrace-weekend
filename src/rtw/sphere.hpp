#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
   public:
    sphere(point3 _center, float _radius, std::shared_ptr<material> _material)
        : center(_center), radius(_radius), mat(_material) {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center - rvec, center + rvec);
    }

    auto hit(const ray& r, interval ray_t, hit_record& rec) const -> bool override {
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
        if (!ray_t.surrounds(root)) {
            // try the other root if the first one is out of range
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root)) return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius;
        auto outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    auto bounding_box() const -> aabb override { return bbox; }

   private:
    point3 center;
    float radius;
    std::shared_ptr<material> mat;
    aabb bbox;
};
