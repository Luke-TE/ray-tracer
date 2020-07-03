#include <iostream>

#include "src/color.h"
#include "src/vec3.h"
#include "src/ray.h"

color ray_color(const ray &r)
{
    vec3 unit_direction = normalize(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 384;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // Viewport details - basically the screen that displays the image
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0; // distance from observer to the viewport

    auto origin = point3(0, 0, 0);
    auto vp_position = vec3(0, 0, focal_length);
    auto vp_x_axis_max = vec3(viewport_width, 0, 0);
    auto vp_y_axis_max = vec3(0, viewport_height, 0);
    auto vp_origin = origin - vp_x_axis_max / 2 - vp_y_axis_max / 2 - vp_position;

    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cerr << "\rProgress: " << static_cast<int>((double)(image_height - 1 - j) / image_height * 100) << ' ' << '%' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            auto u = double(i) / (image_width - 1);  // proportion (0 to 1) along x-axis
            auto v = double(j) / (image_height - 1); // proportion (0 to 1) along y-axis
            ray r(origin, vp_origin + u * vp_x_axis_max + v * vp_y_axis_max - origin);
            // todo: maybe normalise this ^ and check performance

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}