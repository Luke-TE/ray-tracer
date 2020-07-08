#ifndef RT_COMMON_H
#define RT_COMMON_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>
#include <chrono>

// Usings

using std::make_shared;
using std::numeric_limits;
using std::shared_ptr;
using std::sqrt;

// Constats

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

inline double random_double()
{
    // Uniform Real Distribution
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Mersenne Twister 19937 pseudo-random generator
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline double random_axis()
{
    // Uniform Int Distribution
    static std::uniform_int_distribution<int> distribution(0, 2);

    // Mersenne Twister 19937 pseudo-random generator
    static std::mt19937 generator;

    return distribution(generator);
}

inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

// Common Headers

#include "ray.h"
#include "vec3.h"

#endif