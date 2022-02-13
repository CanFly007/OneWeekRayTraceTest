#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list :public hittable
{
public:
	hittable_list() {};
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& output_box)const override;

public:
	std::vector<shared_ptr<hittable>> objects;
};
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	hit_record tmp_record;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (const shared_ptr<hittable>& object : objects)
	{
		if (object->hit(r, t_min, closest_so_far, tmp_record))
		{
			hit_anything = true;
			closest_so_far = tmp_record.t;//射线打到一个物体，此时将max设置为打到物体的，如果再打到更近的才会更新
			rec = tmp_record;
		}
	}
	return hit_anything;
}

bool hittable_list::bounding_box(double t0, double t1, aabb& output_box)const
{
	if (objects.empty())
		return false;

	aabb temp_box;
	bool first_box = true;

	for (const auto& object : objects)
	{
		if (!object->bounding_box(t0, t1, temp_box))
			return false;
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}

#endif // !HITTABLE_LIST_H
