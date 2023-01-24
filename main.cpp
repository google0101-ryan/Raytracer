#include <vector>
#include <fstream>
#include <stdlib.h>

#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "hitable.h"
#include "sphere.h"
#include "hitable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation*color(scattered, world, depth+1);
        }
        else
            return vec3(0, 0, 0);
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable* random_world()
{
    int n = 500;
    hitable** list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++)
    { 
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(),drand48()*drand48(),drand48()*drand48())));
                else if (choose_mat < 0.95)
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.5*drand48()));
                else
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
            }
        }   
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i);
}

int main(void)
{

    std::ofstream out("out.ppm");

    out << "P3\n" << 200 << " " << 100 << "\n255\n";

    camera cam(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 20, float(200) / float(100));

    hitable* world = random_world();

    float p = 0;
    for (int j = 99; j >= 0; j--)
    {
        for (int i = 0; i < 200; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < 10; s++)
            {
                float u = float(i + drand48()) / 200.0;
                float v = float(j + drand48()) / 100.0;
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= 10.0;
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.9*col[0]);
            int ig = int(255.9*col[1]);
            int ib = int(255.9*col[2]);

            out << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}