#ifndef HITTABLE_H
#define HITTABLE_H

#include "rt_common.h"
#include "aabb.h"

// note: to remove circular dependency
class material;

struct hit_record
{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    // checks if ray hits object
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;

    // check if object has a bounding box and, if so, return it
    virtual bool bounding_box(double t0, double t1, aabb &output_box) const = 0;
};

#endif