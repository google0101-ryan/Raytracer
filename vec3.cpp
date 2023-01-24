#include "vec3.h"

inline vec3 &vec3::operator/=(const int v2)
{
    e[0] /= v2;
    e[1] /= v2;
    e[2] /= v2;
    return *this;
}

void vec3::make_unit_vector()
{
    float k = 1.0 / length();
    e[0] *= k; e[1] *= k; e[2] *= k;
}