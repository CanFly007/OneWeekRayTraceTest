#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const = 0;
};

class lambertian :public material
{
public:
	lambertian(const vec3& a) :albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		atten = albedo;
		scattered = ray(record.point, vec3(record.normal + random_unit_vector()));
		return true;
	}
public:
	vec3 albedo;
};

class metal :public material
{
public:
	metal(const vec3& a, double f) :albedo(a), fuzz(f) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		atten = albedo;
		vec3 reflected = reflect(unit_vecotr(r_in.direction()), record.normal);
		scattered = ray(record.point, reflected + fuzz * random_in_unit_sphere());
		return (dot(scattered.direction(), record.normal) > 0);//dot < 0我们认为吸收
	}
public:
	vec3 albedo;
	double fuzz;
};

class dielectric :public material
{
public:
	dielectric(double r) :ref_idx(r) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		atten = vec3(1.0, 1.0, 1.0);
		double etai_over_etat = (record.front_face) ? (1.0 / ref_idx) : (ref_idx);

		vec3 unit_direction = unit_vecotr(r_in.direction());
		double cos_theta = ffmin(dot(-unit_direction, record.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		if (etai_over_etat * sin_theta > 1.0) {
			vec3 reflected = reflect(unit_direction, record.normal);
			scattered = ray(record.point, reflected);
			return true;
		}
		double reflect_prob = schlick(cos_theta, etai_over_etat);
		if (random_double() < reflect_prob)
		{
			vec3 reflected = reflect(unit_direction, record.normal);
			scattered = ray(record.point, reflected);
			return true;
		}
		vec3 refracted = refract(unit_direction, record.normal, etai_over_etat);
		scattered = ray(record.point, refracted);
		return true;
	}
public:
	double ref_idx;
private:
	static double schlick(double cosine, double ref_idx) 
	{
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
#endif // !MATERIAL_H
