#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "material.h"

class sphere : public hittable
{
public:
    sphere() {}
    sphere(const point3 &center, double radius, shared_ptr<material> m)
        : radius(radius), center(center), mat_ptr(m) {}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const;
    virtual bool bounding_box(double t0, double t1, aabb &output_box) const;
    bool set_hit_record(double root, const ray &r, double t_min, double t_max, hit_record &rec) const;

public:
    double radius;
    point3 center;
    shared_ptr<material> mat_ptr;
};

bool sphere::set_hit_record(double root, const ray &r, double t_min, double t_max, hit_record &rec) const
{
    if (root < t_max && root > t_min)
    {
        rec.t = root;
        rec.p = r.at(root);
        auto outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
    }
    return false;
}

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 d = r.origin() - center;

    // based on (simplified) equation
    auto a = r.direction().length_squared();
    auto b_over_2 = dot(d, r.direction());
    auto c = d.length_squared() - radius * radius;

    auto discriminant = b_over_2 * b_over_2 - a * c;

    if (discriminant > 0)
    {
        auto dmnt_sqrted = sqrt(discriminant);
        auto first_root = (-b_over_2 - dmnt_sqrted) / a;
        auto second_root = (-b_over_2 + dmnt_sqrted) / a;
        return set_hit_record(first_root, r, t_min, t_max, rec) || set_hit_record(second_root, r, t_min, t_max, rec);
    }
    return false;
}

bool sphere::bounding_box(double t0, double t1, aabb &output_box) const
{
    output_box = aabb(center - vec3(radius, radius, radius),
                      center + vec3(radius, radius, radius));
    return true;
}

#endif