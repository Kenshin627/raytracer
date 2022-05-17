#ifndef TEXTURE_H
#include "vec3.h"

class texture {
public:
	virtual color3 value(double u, double v, const vec3& point) = 0;
};

class solid_color : public texture {
public:
	solid_color() {}
	solid_color(color3 c): color(c) {}
	solid_color(double r, double b, double g) : solid_color(color3(r, g, b)) {}
	virtual color3 value(double u, double v, const vec3& point) {
		return color;
	}
public:
	color3 color;
};

class checker_texture :public texture {
public:
	checker_texture() {}
	checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd) : even(_even), odd(_odd){}
	checker_texture(color3 c1, color3 c2) : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}
	virtual color3 value(double u, double v, const vec3& p) {
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
		{
			return odd->value(u, v, p);
		}
		else {
			return even->value(u, v, p);
		}
	}
public:
	shared_ptr<texture> even;
	shared_ptr<texture> odd;
};
#define TEXTURE_H
#endif // !TEXTURE_H
