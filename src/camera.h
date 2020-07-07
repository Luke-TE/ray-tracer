#ifndef CAMERA_H
#define CAMERA_H

#include "rt_common.h"

class camera
{
public:
    camera(point3 look_from, point3 look_at, vec3 vup, double vfov,
           double aspect_ratio, double aperture)
    {
        // todo: change to horizontal fov (or do both)
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        // Viewport details - basically the screen that displays the image
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focus_dist = (look_from - look_at).length();

        w = normalize(look_from - look_at); // reverse looking direction
        // projecting vup onto the plane orthogonal to w via cross-product twice
        u = normalize(cross(vup, w));
        v = cross(w, u);

        origin = look_from;
        vp_x_axis = focus_dist * viewport_width * u;
        vp_y_axis = focus_dist * viewport_height * v;

        vp_origin = origin - vp_x_axis / 2 - vp_y_axis / 2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const
    {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        // s = proportion along x axis
        // t = proportion along y axis
        return ray(origin + offset,
                   vp_origin + s * vp_x_axis + t * vp_y_axis - origin - offset);
    }

private:
    point3 origin;
    point3 vp_origin;
    vec3 vp_x_axis;
    vec3 vp_y_axis;
    vec3 u, v, w;
    double lens_radius;
};

#endif