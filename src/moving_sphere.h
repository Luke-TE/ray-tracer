#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rt_common.h"

#include "hittable.h"
#include "sphere.h"

class moving_sphere : public hittable
{
public:
    moving_sphere() {}
    // todo: variable radius?
    moving_sphere(
        point3 starting_center,
        point3 ending_center,
        double t0,
        double t1,
        double r0,
        double r1,
        shared_ptr<material> m)
        : starting_sphere(starting_center, r0, m),
          ending_sphere(ending_center, r1, m),
          time0(t0),
          time1(t1),
          mat_ptr(m)
    {
    }

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const;
    virtual bool bounding_box(double t0, double t1, aabb &output_box) const;

    point3 center(double time) const;
    double moving_sphere::radius(double time) const;

public:
    sphere starting_sphere;
    sphere ending_sphere;
    double time0, time1;
    shared_ptr<material> mat_ptr;
};

point3 moving_sphere::center(double time) const
{
    return starting_sphere.center +
           ((time - time0) / (time1 - time0)) * (ending_sphere.center - starting_sphere.center);
}

double moving_sphere::radius(double time) const
{
    return starting_sphere.radius +
           ((time - time0) / (time1 - time0)) * (ending_sphere.radius - starting_sphere.radius);
}

bool moving_sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    return sphere(center(r.time()), radius(r.time()), mat_ptr).hit(r, t_min, t_max, rec);
}

bool moving_sphere::bounding_box(double t0, double t1, aabb &output_box) const
{
    aabb starting_box, ending_box;
    starting_sphere.bounding_box(t0, t1, starting_box);
    ending_sphere.bounding_box(t0, t1, ending_box);
    output_box = surrounding_box(starting_box, ending_box);
    return true;
}

#endif