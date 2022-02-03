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
#endif // !MATERIAL_H
