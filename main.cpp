#include "src/rt_common.h"

#include <iostream>

#include "src/hittable_list.h"
#include "src/color.h"
#include "src/sphere.h"
#include "src/camera.h"

color ray_color(const ray &r, const hittable &world, int bounces_left)
{
    hit_record rec;

    // Used to limit recursion depth
    if (bounces_left <= 0)
    {
        return color(0.0, 0.0, 0.0);
    }

    double reflection = 0.5;

    // if ray intersects an object, do a different colour
    if (world.hit(r, 0.001, infinity, rec))
    {
        // note: 0.001 used to ignore floating point errors and remove shadow acne
        point3 target = rec.p + rec.normal + random_unit_vector();
        ray reflected_ray(rec.p, target - rec.p);
        return reflection * ray_color(reflected_ray, world, bounces_left - 1); // colour mapping
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

    const int samples_per_pixel = 100; // antialiasing resolution
    const int max_bounces = 50;        // recursion depth

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));      // centre sphere
    world.add(make_shared<sphere>(point3(50, 0.5, -50), 40));   // right sphere
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); // ground

    camera cam;

    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cerr << "\rProgress: " << static_cast<int>((double)(image_height - 1 - j) / image_height * 100) << ' ' << '%' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (i + random_double() - 0.5) / (image_width - 1);  // proportion (0 to 1) along x-axis (randomized)
                auto v = (j + random_double() - 0.5) / (image_height - 1); // proportion (0 to 1) along y-axis (randomized)
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_bounces);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}