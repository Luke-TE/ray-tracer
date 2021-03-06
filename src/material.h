#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt_common.h"
#include "color.h"
#include "hittable.h"

class material
{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color &a) : albedo(a) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        // Different Lambertian Diffuses
        // vec3 scatter_direction = rec.normal + random_in_unit_sphere();
        // vec3 scatter_direction = random_in_hemisphere(rec.normal);
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(scatter_direction, rec.p, r_in.time());
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

class metal : public material
{
public:
    // note: fuzz = variance, fuzz = 0 -> no variance
    metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
        scattered = ray(reflected + fuzz * random_in_unit_sphere(), rec.p, r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

class dielectric : public material
{
public:
    dielectric(double ri) : ref_idx(ri) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        attenuation = color(1.0, 1.0, 1.0);
        double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : (ref_idx);

        vec3 unit_direction = normalize(r_in.direction());

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        vec3 r_out;

        if ((etai_over_etat * sin_theta) > 1.0 || random_double() < schlick(cos_theta, etai_over_etat))
        {
            // reflect ray
            // total internal reflection or based on probability
            r_out = reflect(unit_direction, rec.normal);
        }
        else
        {
            // refract ray
            r_out = refract(unit_direction, rec.normal, etai_over_etat);
        }

        scattered = ray(r_out, rec.p, r_in.time());
        return true;
    }

public:
    double ref_idx;
};

#endif