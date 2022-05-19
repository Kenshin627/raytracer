#ifndef RECT_H
#define RECT_H

#include "hittable.h"
#include "vec3.h"

class rect : public hittable {
public:
	rect() {}
	rect(double x0, double x1, double y0, double y1, double k, shared_ptr<material> m) : x0(x0), x1(x1), y0(y0), y1(y1), k(k), mat_ptr(m) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool boundingbox(aabb& out) const override;
public:
	double x0, x1, y0, y1, k;
	shared_ptr<material> mat_ptr;
};

bool rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double t = (k - r.origin().z()) / r.direction().z();
	if (t < t_min || t > t_max)
	{
		return false;
	}
	double tx = r.origin().x() + t * r.direction().x();
	double ty = r.origin().y() + t * r.direction().y();
	if (tx < x0 || tx > x1 || ty < y0 || ty > y1)
	{
		return false;
	}
	rec.t = t;
	rec.u = (tx - x0) / (x1 - x0);
	rec.v = (ty - y0) / (y1 - y0);
	vec3 out_normal = vec3(0, 0, 1);
	rec.set_face_normal(r, out_normal);
	rec.mat_ptr = mat_ptr;
	rec.p = r.at(t);
	return true;
}

bool rect::boundingbox(aabb& out) const {
	out = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
	return true;
}
#endif // !RECT_H
