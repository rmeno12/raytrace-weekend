#include <iostream>

#include "color.hpp"
#include "vec3.hpp"

auto main() -> int {
    int image_width = 256;
    int image_height = 256;

    std::cout << "P3\n" << image_width << " " << image_height << "\n256\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(static_cast<float>(i) / (image_width - 1),
                                     static_cast<float>(j) / (image_height - 1), 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
