#include "src/rt_common.h"

#include <iostream>

#include "src/hittable_list.h"
#include "src/color.h"
#include "src/sphere.h"

using std::numeric_limits;

color ray_color(const ray &r, const hittable &world)
{
    hit_record rec;

    // if ray intersects an object, do a different colour
    if (world.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + color(1, 1, 1)); // colour mapping
    }

    // background colour
    // calculating a colour based on the ray's position
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

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));      // centre sphere
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); // ground

    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cerr << "\rProgress: " << static_cast<int>((double)(image_height - 1 - j) / image_height * 100) << ' ' << '%' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            auto u = double(i) / (image_width - 1);  // proportion (0 to 1) along x-axis
            auto v = double(j) / (image_height - 1); // proportion (0 to 1) along y-axis
            ray r(origin, vp_origin + u * vp_x_axis_max + v * vp_y_axis_max - origin);
            // todo: maybe normalise this ^ and check performance

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}