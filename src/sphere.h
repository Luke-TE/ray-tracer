#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"

class sphere
{
public:
    sphere(const point3 &center, double radius)
        : r(radius), c(center)
    {
    }

    double radius() const { return r; }
    vec3 center() const { return c; }

public:
    double r;
    vec3 c;
};

#endif