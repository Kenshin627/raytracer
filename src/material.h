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
	metal(const color3& a, double f) : albedo(a), fuzz(f < 1? f: 1) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const override {
		auto scatter_direction = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, scatter_direction + fuzz * random_unitVec() );
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
public:
	color3 albedo;
	double fuzz;
};

class dietectirc : public material {
public:
	dietectirc(double index_of_refractio) : ir(index_of_refractio) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const override {
		double refraction_radio = rec.front_face ? 1.0 / ir : ir;
		auto cos_theta = fmin(dot(-unit_vector(r_in.direction()), rec.normal), 1.0);
		auto sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		vec3 scatter_direction;
		bool totalInternalReflect = sin_theta * refraction_radio > 1.0;
		if (totalInternalReflect || reflectance(cos_theta, refraction_radio) > random_double())
		{
			scatter_direction = reflect(unit_vector(r_in.direction()), rec.normal);
		}
		else {
			scatter_direction = refract(unit_vector(r_in.direction()), rec.normal, refraction_radio);
		}
		attenuation = color3(1.0, 1.0, 1.0);
		scattered = ray(rec.p, scatter_direction);
		return true;
	}
public:
	double ir;
private:
	static double reflectance(double cosine, double ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

#endif // !MATERIAL_H
