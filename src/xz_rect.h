#ifndef XZ_RECT_H
#define XZ_RECT_H

#include "hittable.h"
#include "vec3.h"

class xz_rect : public hittable {
public:
	xz_rect() {}
	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> _m) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(_m) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool boundingbox(aabb& out) const override;
public:
	double x0, x1, z0, z1, k;
	shared_ptr<material> mat_ptr;
};

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double t = (k - r.origin().y()) / r.direction().y();
	if (t <  t_min || t > t_max)
	{
		return false;
	}

	double tx = r.origin().x() + t * r.direction().x();
	double tz = r.origin().z() + t * r.direction().z();
	if (tx < x0 || tx > x1 || tz < z0 || tz > z1)
	{
		return false;
	}
	rec.t = t;
	rec.u = (tx - x0) / (x1 - x0);
	rec.v = (tz - z0) / (z1 - z0);
	rec.p = r.at(t);
	vec3 out_normal = vec3(0, 1, 0);
	rec.mat_ptr = mat_ptr;
	rec.set_face_normal(r, out_normal);
	return true;
}

bool xz_rect::boundingbox(aabb& out) const {
	out = aabb(vec3(x0, k - 0.001, z0), vec3(x1, k - 0.001, z1));
	return true;
}

#endif // !XZ_RECT_H

