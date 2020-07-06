#ifndef CAMERA_H
#define CAMERA_H

#include "rt_common.h"

class camera
{
public:
    camera()
    {
        const auto aspect_ratio = 16.0 / 9.0;

        // Viewport details - basically the screen that displays the image
        const auto viewport_height = 2.0;
        const auto viewport_width = aspect_ratio * viewport_height;
        const auto focal_length = 1.0; // distance from observer to the viewport
        const auto viewport_pos = vec3(0, 0, focal_length);

        origin = point3(0, 0, 0);
        vp_x_axis_max = vec3(viewport_width, 0, 0);
        vp_y_axis_max = vec3(0, viewport_height, 0);

        vp_origin = origin - vp_x_axis_max / 2 - vp_y_axis_max / 2 - viewport_pos;
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