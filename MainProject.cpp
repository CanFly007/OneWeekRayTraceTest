#include<iostream>

#include "sphere.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "camera.h"
#include "material.h"

using std::make_shared;

vec3 ray_color(const ray& r,const hittable_list& world,int depth)
{
	if (depth <= 0)
		return vec3(0, 0, 0);

	hit_record record;
	if (world.hit(r, 0.001, infinity, record))
	{
		vec3 atten;
		ray scattered;
		if (record.mat_ptr->scatter(r, record, atten, scattered))
		{
			return atten * ray_color(scattered, world, depth - 1);
		}
		return vec3(0, 0, 0);
	}
	vec3 unit_direction = unit_vecotr(r.direction());
	double t = (unit_direction.y() + 1.0) * 0.5;
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void main()
{
	const int image_width = 200;
	const int image_height = 100;
	const int samplers_per_pixel = 100;
	const int max_depth = 50;

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	hittable_list world;
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.7, 0.3, 0.3))));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.8, 0.8, 0.0))));

	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 1.0)));
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.8, 0.8), 0.3)));
	camera cam;

	for (int j = image_height - 1; j >= 0; j--)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			vec3 color(0, 0, 0);
			for (int s = 0; s < samplers_per_pixel; s++)
			{
				double u = (i + random_double()) / image_width;
				double v = (j + random_double()) / image_height;
				ray r = cam.get_ray(u, v);
				color += ray_color(r, world, max_depth);
			}
			color.write_color(std::cout, samplers_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}