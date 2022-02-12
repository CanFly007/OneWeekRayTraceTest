#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"

#include "hittable.h"

class moving_sphere :public hittable
{
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, double t0, double t1, double r, shared_ptr<material> m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;

	vec3 center(double time)const;
public:
	vec3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;
};

vec3 moving_sphere::center(double time)const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	vec3 op = r.origin() - center(r.time());
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
			vec3 outwardNormal = (rec.point - center(r.time())) / radius;
			rec.set_face_normal(r, outwardNormal);//如果从内部射入，法线也是朝向内部的
			rec.mat_ptr = mat_ptr;
			return true;
		}
		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.point = r.at(t);
			vec3 outwardNormal = (rec.point - center(r.time())) / radius;
			rec.set_face_normal(r, outwardNormal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

#endif // !MOVING_SPHERE_H
