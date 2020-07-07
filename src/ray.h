#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
    ray() {}
    ray(const vec3 &direction,
        const point3 &origin = point3(0.0, 0.0, 0.0),
        double time = 0.0)
        : dir(direction), orig(origin), tm(time) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    double time() const { return tm; }

    point3 at(double t) const { return orig + t * dir; }

public:
    point3 orig;
    vec3 dir;
    double tm;
};

#endif