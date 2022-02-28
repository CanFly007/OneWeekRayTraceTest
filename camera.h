#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double fov, double aspect, double aperture, double focus_disk,
	double t0 = 0,double t1 = 0)
	{
		origin = lookfrom;
		len_radius = aperture / 2;
		time0 = t0;
		time1 = t1;
		double radians = degrees_to_radians(fov);
		double halfHeight = tan(radians * 0.5);
		double halfWidth = halfHeight * aspect;

		z = unit_vecotr(lookfrom - lookat);
		x = unit_vecotr(cross(vup, z));
		y = cross(z, x);

		lower_left_corner = origin - halfWidth * x * focus_disk - halfHeight * y * focus_disk - z * focus_disk;
		horizontal = 2 * halfWidth * x * focus_disk;
		vertical = 2 * halfHeight * y * focus_disk;
	}

	ray get_ray(double u, double v)
	{
		vec3 rd = len_radius * random_in_unit_disk();
		vec3 offset = x * rd.x() + y * rd.y();
		return ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset
		,random_double(time0,time1));
	}
public:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 x, y, z;
	double len_radius;
	double time0, time1;
};
#endif // !CAMERA_H

