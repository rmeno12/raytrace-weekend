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

class dielectric : public material {
   public:
    dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    auto scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
        -> bool override {
        attenuation = color(1.0, 1.0, 1.0);
        float refraction_ratio = rec.front_face ? 1.0 / ir : ir;

        auto unit_direction = unit_vector(r_in.direction());
        float cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        float sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction);

        return true;
    }

   private:
    float ir;

    static auto reflectance(float cosine, float ref_idx) -> double {
        // Schlick's approximation
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 *= r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};
