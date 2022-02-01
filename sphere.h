#ifndef SPHERE_H
#define SPHERE_H

#include"hittable.h"
#include"vec3.h"

class sphere :public hittable
{
public:
	sphere();
	sphere(vec3 cen, double r, shared_ptr<material> m) :center(cen), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;
public:
	vec3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	vec3 op = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = 2.0 * dot(op, r.direction());
	double c = dot(op, op) - radius * radius;
	double discriminant = b * b - 4 * a * c;
	if (discriminant > 0)
	{
		double t = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.point = r.at(t);
			vec3 outwardNormal = (rec.point - center) / radius;
			rec.set_face_normal(r, outwardNormal);//如果从内部射入，法线也是朝向内部的
			rec.mat_ptr = mat_ptr;
			return true;
		}
		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.point = r.at(t);
			vec3 outwardNormal = (rec.point - center) / radius;
			rec.set_face_normal(r, outwardNormal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

#endif // !SPHERE_H

