#ifndef CAMERA_H
#define CAMERA_H

#include "rt_common.h"

class camera
{
public:
    camera(point3 look_from, point3 look_at, vec3 vup, double vfov, double aspect_ratio)
    {
        // todo: change to horizontal fov (or do both)
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        // Viewport details - basically the screen that displays the image
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = normalize(look_from - look_at); // reverse looking direction
        // projecting vup onto the plane orthogonal to w via cross-product twice
        auto u = normalize(cross(vup, w));
        auto v = cross(w, u);

        origin = look_from;
        vp_x_axis_max = viewport_width * u;
        vp_y_axis_max = viewport_height * v;

        vp_origin = origin - vp_x_axis_max / 2 - vp_y_axis_max / 2 - w;
    }

    ray get_ray(double u, double v) const
    {
        return ray(origin, vp_origin + u * vp_x_axis_max + v * vp_y_axis_max - origin);
    }

private:
    point3 origin;
    point3 vp_origin;
    vec3 vp_x_axis_max;
    vec3 vp_y_axis_max;
};

#endif