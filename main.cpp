#include "src/rt_common.h"

#include <iostream>

#include "src/hittable_list.h"
#include "src/color.h"
#include "src/sphere.h"
#include "src/moving_sphere.h"
#include "src/camera.h"
#include "src/material.h"
#include "src/bvh.h"

color ray_color(const ray &r, const hittable &world, int bounces_left)
{
    hit_record rec;

    // Used to limit recursion depth
    if (bounces_left <= 0)
    {
        return color(0, 0, 0);
    }

    double reflection = 0.5;

    // if ray intersects an object, do a different colour
    if (world.hit(r, 0.001, infinity, rec))
    {
        // note: 0.001 used to ignore floating point errors and remove shadow acne

        ray scattered_ray;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered_ray))
        {
            return attenuation * ray_color(scattered_ray, world, bounces_left - 1);
        }
        return color(0, 0, 0);
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
    const int image_width = 768;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    const int samples_per_pixel = 100; // antialiasing resolution
    const int max_bounces = 50;        // recursion depth

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    hittable_list world;

    // lambertian spheres
    auto lamb_a = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    // world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, lamb_a)); // centre sphere
    world.add(make_shared<moving_sphere>(point3(0, 0, -1), point3(0, 1, -1), 0, 1, 0.5, 0.75, lamb_a)); // centre sphere
    auto lamb_b = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, lamb_b)); // ground

    // dielectric sphere
    auto dielectric_a = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(1, 0, -0.9), 0.5, dielectric_a)); // right sphere

    // hollow sphere bubble
    // negative radius -> same geometry, inverted surface normals
    auto dielectric_b = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(1, 0, -0.9), -0.45, dielectric_b)); // right sphere

    // metal sphere
    auto metal_a = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    world.add(make_shared<sphere>(point3(-1, 0, -1.2), 0.5, metal_a)); // left sphere

    point3 look_from(4, 3, 2);
    point3 look_at(0, 0, -1);
    vec3 vup(0, 1, 0);
    auto vfov = 20;
    auto aperture = 0.5;

    camera cam(look_from, look_at, vup, vfov, aspect_ratio, aperture, 0, 1);

    bvh_node world_bvh(world, 0, 1);

    auto starting_time = std::chrono::high_resolution_clock::now();

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
                pixel_color += ray_color(r, world_bvh, max_bounces);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    auto ending_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = ending_time - starting_time;

    std::cerr << "\nDone.\n";
    std::cerr << "Finished in " << elapsed_time.count() << " seconds\n";
}