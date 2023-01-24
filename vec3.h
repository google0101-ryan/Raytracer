#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3
{
public:
    vec3() {}
    vec3(float e0, float e1, float e2) {e[0] = e0; e[1] = e1; e[2] = e2;}
    inline float x() {return e[0];}
    inline float y() {return e[1];}
    inline float z() {return e[2];}
    inline float r() {return e[0];}
    inline float g() {return e[1];}
    inline float b() {return e[2];}

    inline const vec3& operator+() const {return *this;}
    inline vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
    inline float operator[](int i) const {return e[i];}
    inline float& operator[](int i) {return e[i];}

    inline vec3& operator+=(const vec3& v2);
    inline vec3& operator-=(const vec3& v2);
    inline vec3& operator*=(const vec3& v2);
    inline vec3& operator/=(const vec3& v2);
    inline vec3& operator/=(const int v2);

    inline float length() const { return sqrt(e[0]*e[0]+e[1]*e[1]+e[2]*e[2]); }
    inline float squared_length() const { return e[0]*e[0]+e[1]*e[1]+e[2]*e[2]; }
    void make_unit_vector();

    float e[3];
};

inline vec3 operator+(const vec3& v1, const vec3& v2)
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2)
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(float t, const vec3& v)
{
    return vec3(v.e[0]*t, v.e[1]*t, v.e[2]*t);
}

inline float dot(const vec3& v1, const vec3& v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}