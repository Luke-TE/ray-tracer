#ifndef AABB_H
#define AABB_H

#include "rt_common.h"

class aabb
{
public:
    aabb() {}
    aabb(const point3 &a, const point3 &b)
    {
        min = a;
        max = b;
    }

    point3 min_corner() const { return min; }
    point3 max_corner() const { return max; }

    // note: similar to hittable, but with no hit_record
    bool hit(const ray &r, double t_min, double t_max) const
    {
        // todo: implement optimised version by Andrew Kensler
        // for each component
        for (int a = 0; a < 3; a++)
        {
            // get the t interval for intersection

            // t interval lower bound
            auto t0 = (min[a] - r.origin()[a]) / r.direction()[a];
            // t interval upper bound
            auto t1 = (max[a] - r.origin()[a]) / r.direction()[a];

            // take into account negatives
            if (t0 > t1)
            {
                auto temp = t0;
                t0 = t1;
                t1 = temp;
            }

            t_min = fmax(t0, t_min); // get the largest lower bound for t
            t_max = fmin(t1, t_max); // get the smallest upper bound for t

            // if bounds do not make sense (so t ranges do not overlap)
            // no ray intersection
            if (t_max <= t_min)
            {
                return false;
            }
        }

        return true;
    }

    point3 min;
    point3 max;
};

aabb surrounding_box(aabb &box0, aabb &box1)
{
    point3 min_corner(fmin(box0.min_corner().x(), box1.min_corner().x()),
                      fmin(box0.min_corner().y(), box1.min_corner().y()),
                      fmin(box0.min_corner().z(), box1.min_corner().z()));
    point3 max_corner(fmax(box0.max_corner().x(), box1.max_corner().x()),
                      fmax(box0.max_corner().y(), box1.max_corner().y()),
                      fmax(box0.max_corner().z(), box1.max_corner().z()));

    return aabb(min_corner, max_corner);
}

#endif