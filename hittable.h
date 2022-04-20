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
	//每个hittable物体需要一个aabb盒子
	virtual bool bounding_box(double t0, double t1, aabb& output_box)const = 0;
};

class translate :public hittable
{
public:
	translate(shared_ptr<hittable> ptr, const vec3& _offset) :hittablePtr(ptr), offset(_offset) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;
	//每个hittable物体需要一个aabb盒子
	virtual bool bounding_box(double t0, double t1, aabb& output_box)const;

public:
	shared_ptr<hittable> hittablePtr;
	vec3 offset;
};

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	ray moved_r(r.origin() - offset, r.direction(), r.time());
	if (!hittablePtr->hit(moved_r, t_min, t_max, rec))
		return false;

	rec.point += offset;
	rec.set_face_normal(moved_r, rec.normal);
	return true;
}
bool translate::bounding_box(double t0, double t1, aabb& output_box)const
{
	if (!hittablePtr->bounding_box(t0, t1, output_box))
		return false;

	output_box = aabb(output_box.min() + offset, output_box.max() + offset);
	return true;
}
#endif