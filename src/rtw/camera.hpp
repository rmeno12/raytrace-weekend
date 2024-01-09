#pragma once

#include <iostream>
#include <thread>

#include "color.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "rtweekend.hpp"

class camera {
   public:
    float aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    float vfov = 90;
    point3 lookfrom = point3(0, 0, -1);
    point3 lookat = point3(0, 0, 0);
    vec3 vup = vec3(0, 1, 0);

    float defocus_angle = 0;
    float focus_dist = 10;

    auto render(const hittable& world) -> void {
        initialize();

        color* image = new color[image_width * image_height];
        std::vector<std::thread> threads;
        for (int j = 0; j < image_height; j++) {
            // each thread does a row, one thread per j
            color* row = image + j * image_width;
            threads.emplace_back([&, row, j] {
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        auto r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    row[i] = pixel_color;
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "P3\n" << image_width << " " << image_height << "\n256\n";
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                auto pixel_color = image[j * image_width + i];
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        delete[] image;
    }

   private:
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    auto initialize() -> void {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // basis vectors for camera frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    auto get_ray(int i, int j) const -> ray {
        auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = defocus_angle <= 0 ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    auto pixel_sample_square() const -> vec3 {
        auto px = -0.5 + random_float();
        auto py = -0.5 + random_float();

        return px * pixel_delta_u + py * pixel_delta_v;
    }

    auto defocus_disk_sample() const -> point3 {
        auto p = random_in_unit_disk();
        return center + p[0] * defocus_disk_u + p[1] * defocus_disk_v;
    }

    auto ray_color(const ray& r, int depth, const hittable& world) const -> color {
        if (depth <= 0) return color(0, 0, 0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;

            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);

            return color(0, 0, 0);
        }

        auto unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1);
        return (1.0 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
    }
};
