
#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

vec3 ray_color(const ray& r, const hittable& world, int maxDepth) {
    hit_record rec;
    if (maxDepth <= 0)
    {
        return color3(0, 0, 0);
    }
    if (world.hit(r, 0.01, infinity, rec))
    {
        ray scattered;
        color3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, maxDepth--);
        }
        return color3(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.2, 0.7, 1.0);
}



int main()
{
    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    //world
    hittable_list world;
    
    auto material_ground = make_shared<lambertian>(color3(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color3(0.7, 0.3, 0.3));
    auto material_left = make_shared<metal>(color3(0.8, 0.8, 0.8));
    auto material_right = make_shared<metal>(color3(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    //camera
    camera cam;

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
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(file, pixel_color, samples_per_pixel);
        }
    }
    file.close();
}

