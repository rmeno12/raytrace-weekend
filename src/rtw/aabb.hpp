#pragma once

#include "interval.hpp"
#include "ray.hpp"

class aabb {
   public:
    interval x, y, z;

    aabb() {}

    aabb(const interval& ix, const interval& iy, const interval& iz) : x(ix), y(iy), z(iz) {}

    // using a and b as opposite corners of the box
    aabb(const point3& a, const point3& b)
        : x(std::min(a[0], b[0]), std::max(a[0], b[0])),
          y(std::min(a[1], b[1]), std::max(a[1], b[1])),
          z(std::min(a[2], b[2]), std::max(a[2], b[2])) {}

    aabb(const aabb& box0, const aabb& box1)
        : x(interval(box0.x, box1.x)), y(interval(box0.y, box1.y)), z(interval(box0.z, box1.z)) {}

    auto axis(int n) const -> const interval& {
        if (n == 1)
            return y;
        else if (n == 2)
            return z;
        else
            return x;
    }

    auto hit(const ray& r, interval ray_t) const -> bool {
        // andrew kensler's better version
        for (int a = 0; a < 3; a++) {
            auto invD = 1 / r.direction()[a];
            auto orig = r.origin()[a];

            auto t0 = (axis(a).min - orig) * invD;
            auto t1 = (axis(a).max - orig) * invD;

            if (invD < 0) std::swap(t0, t1);

            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 > ray_t.min) ray_t.max = t1;

            if (ray_t.max <= ray_t.min) return false;
        }

        return true;
    }
};
