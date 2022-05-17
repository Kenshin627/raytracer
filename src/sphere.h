#ifndef  SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public: 
	sphere() {}
	sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool boundingbox(aabb& out) const override;
public:
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
private:
	static void get_sphere_uv(const point3& p, double& u, double& v) {
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;
		u = phi / (2 * pi);
		v = theta / pi;
	}
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().squared_length();
	auto half_b = dot(oc, r.direction());
	auto c = oc.squared_length() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) 
	{
		return false;
	}
	auto sqrtd = sqrt(discriminant);

	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || root > t_max)
		{
			return false;
		}
	}

	rec.t = root;
	rec.mat_ptr = mat_ptr;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, rec.u, rec.v);
	return true;
}

bool sphere::boundingbox(aabb& out) const {
	vec3 offset(radius, radius, radius);
	out = aabb(center - offset, center + offset);
	return true;
}


#endif // ! SPHERE_H
