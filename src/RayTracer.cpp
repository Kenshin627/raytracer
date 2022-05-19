﻿
#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "rect.h"

vec3 ray_color(const ray& r, const color3& background, const hittable& world, int maxDepth) {
    hit_record rec;
    if (maxDepth <= 0)
    {
        return color3(0, 0, 0);
    }
    if (!world.hit(r, 0.001, infinity, rec))
    {
        return background;
    }
    ray scattered;
    color3 attenuation;
    color3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    {
        return emitted;
    }
    return emitted + attenuation * ray_color(scattered, background, world, maxDepth-1);
}

hittable_list random_scene() {
    hittable_list world;

    //auto ground_material = make_shared<lambertian>(color3(0.5, 0.5, 0.5));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto checker = make_shared<checker_texture>(color3(0.2, 0.2, 0.2), color3(0.8, 0.8, 0.8));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dietectirc>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dietectirc>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    //return world;
    return hittable_list(make_shared<bvh_node>(world));
}

hittable_list simple_light() {
    hittable_list objects;

    auto check = make_shared<checker_texture>(color3(0.2, 0.2, 0.2), color3(0.8, 0.8, 0.8));
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(check)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(color3(0.2, 0.5, 1.0))));

    auto diffLight = make_shared<diffuseLight>(color3(4, 4, 4));
    objects.add(make_shared<rect>(3, 5, 1, 3, -2, diffLight));
    return objects;
}


int main()
{
    //Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    color3 background(0.0, 0.0, 0.0);
    //world
    hittable_list world = random_scene();
    hittable_list simpleL = simple_light();
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    double fov = 20.0;
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    switch (1)
    {
        case 0:
            world = random_scene();
        case 1:
            world = simple_light();
            background = color3(0, 0, 0);
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            fov = 20.0;
            break;
        default:
            break;
    }

    camera cam(lookfrom, lookat, vup, fov, aspect_ratio, dist_to_focus, aperture);

    //Render
    std::fstream file;

    file.open("output.ppm", std::ios::out);

    file << "P3\n" << image_width << " " << image_height << "\n255\n" << std::endl;

    for (int j = image_height - 1; j >= 0 ; j--)
    {
        for (int i = 0; i < image_width; i++)
        {
            vec3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(file, pixel_color, samples_per_pixel);
        }
    }
    file.close();
}

