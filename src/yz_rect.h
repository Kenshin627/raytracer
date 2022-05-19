#ifndef YZ_RECT_H
#define YZ_RECT_H

#include "hittable.h"
#include "vec3.h"

class yz_rect : public hittable{
public:
	yz_rect() {}
	yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> _m): y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(_m) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool boundingbox(aabb& out) const override;
public:
	double y0, y1, z0, z1, k;
	shared_ptr<material> mat_ptr;
};

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double t = (k - r.origin().x()) / r.direction().x();
	if (t <  t_min || t > t_max)
	{
		return false;
	}

	double ty = r.origin().y() + t * r.direction().y();
	double tz = r.origin().z() + t * r.direction().z();
	if (ty < y0 || ty > y1 || tz < z0 || tz > z1)
	{
		return false;
	}
	rec.t = t;
	rec.u = (ty - y0) / (y1 - y0);
	rec.v = (tz - z0) / (z1 - z0);
	rec.p = r.at(t);
	vec3 out_normal = vec3(-1, 0, 0);
	rec.mat_ptr = mat_ptr;
	rec.set_face_normal(r, out_normal);
	return true;
}

bool yz_rect::boundingbox(aabb& out) const {
	out = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
	return true;
}

#endif // !YZ_RECT_H

