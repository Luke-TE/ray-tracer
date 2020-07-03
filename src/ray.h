#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "sphere.h"

class ray
{
public:
    ray() {}
    ray(const vec3 &direction)
        : orig(vec3(0.0, 0.0, 0.0)), dir(direction)
    {
    }
    ray(const point3 &origin, const vec3 &direction)
        : orig(origin), dir(direction)
    {
    }

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t) const { return orig + t * dir; }

public:
    point3 orig;
    vec3 dir;
};

inline bool intersects_sphere(const ray &r, const sphere &s)
{
    vec3 d = r.origin() - s.center();

    // based on equation
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(d, r.direction());
    auto c = dot(d, d) - s.radius() * s.radius();

    auto discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

#endif