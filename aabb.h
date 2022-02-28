#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb
{
public:
	aabb() {}
	aabb(const vec3& a, const vec3& b) { _min = a; _max = b; }

	vec3 min()const { return _min; }
	vec3 max()const { return _max; }

	bool hit(const ray& r, double tmin, double tmax)const
	{
		for (int xyz = 0; xyz < 3; xyz++)
		{
			double tt0 = (_min[xyz] - r.origin()[xyz]) / r.direction()[xyz];
			double tt1 = (_max[xyz] - r.origin()[xyz]) / r.direction()[xyz];
			//保持t0一直小于t1
			double t0 = ffmin(tt0, tt1);
			double t1 = ffmax(tt0, tt1);

			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			//条件为true，则没有交集
			if (tmax < tmin)
				return false;
		}
		return true;
	}

	vec3 _min;
	vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1)
{
	vec3 small(ffmin(box0.min().x(), box1.min().x()),
			   ffmin(box0.min().y(), box1.min().y()),
		       ffmin(box0.min().z(), box1.min().z()));
	vec3 big(ffmax(box0.max().x(), box1.max().x()),
		     ffmax(box0.max().y(), box1.max().y()),
		     ffmax(box0.max().z(), box1.max().z()));
	return aabb(small, big);
}
#endif