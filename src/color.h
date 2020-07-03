#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream &out, color pixel_color)
{
    // Map each color component 0 to 1 -> 0 to 255 and output it
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    // note: use 255.99, otherwise ix = 255 only
    // when x = 1, not even when x = 0.999
}

#endif