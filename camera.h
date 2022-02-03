#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double fov, double aspect)
	{

		origin = lookfrom;
		double radians = degrees_to_radians(fov);
		double halfHeight = tan(radians * 0.5);
		double halfWidth = halfHeight * aspect;

		vec3 z = unit_vecotr(lookfrom - lookat);
		vec3 x = unit_vecotr(cross(vup, z));
		vec3 y = cross(z, x);

		lower_left_corner = origin - halfWidth * x - halfHeight * y - z;
		horizontal = 2 * halfWidth * x;
		vertical = 2 * halfHeight * y;
	}

	ray get_ray(double u, double v)
	{
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}
public:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};
#endif // !CAMERA_H

