#include <vector>
#include <fstream>
#include <stdlib.h>

#include "vec3.h"
#include "ray.h"
#include "camera.h"

float hit_sphere(const vec3& center, float radius, const ray& r)
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0)
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0 * a);
}

vec3 color(const ray& r)
{
    float t = hit_sphere(vec3(0.0, 0.0, -1.0), 0.5, r);
    if (t > 0.0)
    {
        vec3 N = (r.point_at_parameter(t) - vec3(0.0, 0.0, -1.0));
        N.make_unit_vector();
        return 0.5*vec3(N.r()+1, N.g()+1, N.b()+1);
    }
    vec3 unit_direction = r.direction();
    unit_direction.make_unit_vector();
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main(void)
{

    std::ofstream out("out.ppm");

    out << "P3\n" << 200 << " " << 100 << "\n255\n";

    camera cam;
    for (int j = 99; j >= 0; j--)
    {
        for (int i = 0; i < 200; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < 100; s++)
            {
                float u = float(i + drand48()) / 200;
                float v = float(j + drand48()) / 100;
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r);
            }
            col /= 100;
            int ir = int(255.9*col[0]);
            int ig = int(255.9*col[1]);
            int ib = int(255.9*col[2]);

            out << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}