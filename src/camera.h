#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
	camera() { }
	camera(const point3& from, const vec3& lookAt, const vec3& up, const double fov, const double aspect_ratio, const double focal, double aperture) {
		origin = from;
		auto z = unit_vector(origin - lookAt);
		auto x = unit_vector(cross(up, z));
		auto y = unit_vector(cross(z, x));
		double fovRadian = degrees_to_radians(fov);
		double viewport_H = tan(fovRadian / 2) *  2;
		double viewport_W = aspect_ratio * viewport_H;
		horizontal = viewport_W * x * focal;
		vertical = viewport_H * y * focal;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - z * focal;
		lens_radius = aperture / 2;
	}

	ray get_ray(double u, double v) const {

		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = x * rd.x() + y * rd.y();
		return ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset);
	}

private:
	point3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 x, y, z;
	double lens_radius;
};
#endif // !CAMERA_H

