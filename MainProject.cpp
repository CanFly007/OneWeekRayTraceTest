#include<iostream>

#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"

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

hittable_list random_scene()
{
	hittable_list world;

	world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					// diffuse
					auto albedo = vec3::random() * vec3::random();
					world.add(
						make_shared<moving_sphere>(center, 
							center + vec3(0, random_double(0, 0.5), 0), 0.0, 1.0,
							0.2, make_shared<lambertian>(albedo)));
				}
				else if (choose_mat < 0.95)
				{
					// metal
					auto albedo = vec3::random(.5, 1);
					auto fuzz = random_double(0, .5);
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
				}
				else
				{
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

	world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return static_cast<hittable_list>(make_shared<bvh_node>(world, 0, 1));
	return world;
}

void main()
{
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 120;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samplers_per_pixel = 100;
	const int max_depth = 50;

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	
	//camera
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10;
	auto aperture = 0.1;

	hittable_list world = random_scene();
	camera cam(lookfrom, lookat, vup, 20, double(image_width) / image_height, aperture, dist_to_focus);

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

