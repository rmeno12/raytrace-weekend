#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "ray.hpp"

class material {
   public:
    virtual ~material() = default;

    virtual auto scatter(const ray& r_in, const hit_record& rec, color& attenuation,
                         ray& scattered) const -> bool = 0;
};

class lambertian : public material {
   public:
    lambertian(const color& a) : albedo(a) {}

    auto scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
        -> bool override {
        auto scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero()) scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;

        return true;
    }

   private:
    color albedo;
};

class metal : public material {
   public:
    metal(const color& a, float f) : albedo(a), fuzz(f) {}

    auto scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
        -> bool override {
        auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
        attenuation = albedo;

        return dot(scattered.direction(), rec.normal) > 0;
    }

   private:
    color albedo;
    float fuzz;
};
