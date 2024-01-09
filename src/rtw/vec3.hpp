#pragma once

#include <cmath>
#include <iostream>

#include "rtweekend.hpp"

class vec3 {
   public:
    float e[3];

    vec3() : e{0, 0, 0} {}
    vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

    auto x() const -> float { return e[0]; }
    auto y() const -> float { return e[1]; }
    auto z() const -> float { return e[2]; }

    auto operator-() const -> vec3 { return vec3(-e[0], -e[1], -e[2]); }
    auto operator[](size_t i) const -> float { return e[i]; }
    auto operator[](size_t i) -> float& { return e[i]; }

    auto operator+=(const vec3& v) -> vec3& {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    auto operator*=(float t) -> vec3& {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    auto operator/=(float t) -> vec3& { return *this *= 1 / t; }

    auto length() const -> float { return std::sqrt(length_squared()); }

    auto length_squared() const -> float { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    auto near_zero() const -> bool {
        auto s = 1e-8;
        return std::fabs(e[0]) < s && std::fabs(e[1]) < s && std::fabs(e[2]) < s;
    }

    static auto random() -> vec3 { return vec3(random_float(), random_float(), random_float()); }

    static auto random(float min, float max) -> vec3 {
        return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
    }
};

using point3 = vec3;

inline auto operator<<(std::ostream& out, const vec3& v) -> std::ostream& {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline auto operator+(const vec3& u, const vec3& v) -> vec3 {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline auto operator-(const vec3& u, const vec3& v) -> vec3 {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline auto operator*(const vec3& u, const vec3& v) -> vec3 {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline auto operator*(const vec3& v, float t) -> vec3 {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline auto operator*(float t, const vec3& v) -> vec3 { return v * t; }

inline auto operator/(const vec3& v, float t) -> vec3 { return (1 / t) * v; }

inline auto dot(const vec3& u, const vec3& v) -> float {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline auto cross(const vec3& u, const vec3& v) -> vec3 {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline auto unit_vector(const vec3& v) -> vec3 { return v / v.length(); }

inline auto random_in_unit_sphere() -> vec3 {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() < 1) return p;
    }
}

inline auto random_unit_vector() -> vec3 { return unit_vector(random_in_unit_sphere()); }

inline auto random_on_hemisphere(const vec3& normal) -> vec3 {
    auto on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline auto reflect(const vec3& v, const vec3& n) -> vec3 { return v - 2 * dot(v, n) * n; }

inline auto refract(const vec3& uv, const vec3& n, double etai_over_etat) -> vec3 {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    auto r_out_perp = etai_over_etat * (uv + cos_theta * n);
    auto r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;

    return r_out_perp + r_out_parallel;
}
