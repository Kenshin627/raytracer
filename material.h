#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

#include "hittable.h"

class material {

public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color3& attenution, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unitVec();
		if (scatter_direction.near_zero())
		{
			scatter_direction = rec.normal;
		}
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
public:
	color3 albedo;
};

class metal : public material {
public:
	metal(const color3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const override {
		auto scatter_direction = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
public:
	color3 albedo;
};
#endif // !MATERIAL_H
