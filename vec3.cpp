#include "vec3.h"

vec3 &vec3::operator+=(const vec3 &v2)
{
    e[0] += v2.e[0];
    e[1] += v2.e[1];
    e[2] += v2.e[2];
    return *this;
}

vec3 &vec3::operator/=(const float t)
{
    float k = 1.0 / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

void vec3::make_unit_vector()
{
    float k = 1.0 / length();
    e[0] *= k; e[1] *= k; e[2] *= k;
}