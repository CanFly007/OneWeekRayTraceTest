#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"

class material;

struct hit_record
{
	vec3 point;
	vec3 normal;
	double t;
	double u;
	double v;
	bool front_face;
	shared_ptr<material> mat_ptr;

	inline void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
	//ÿ��hittable������Ҫһ��aabb����
	virtual bool bounding_box(double t0, double t1, aabb& output_box)const = 0;
};
#endif