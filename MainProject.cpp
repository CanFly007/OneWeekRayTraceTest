#include<iostream>

#include "sphere.h"
#include "hittable_list.h"
#include "rtweekend.h"

using std::make_shared;

vec3 ray_color(const ray& r,const hittable_list& world)
{
	hit_record record;
	if (world.hit(r, 0, infinity, record))
	{
		return (record.normal + 1.0) * 0.5;
	}
	vec3 unit_direction = unit_vecotr(r.direction());
	double t = (unit_direction.y() + 1.0) * 0.5;
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void main()
{
	const int image_width = 200;
	const int image_height = 100;

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	hittable_list world;
	//world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

	for (int j = image_height - 1; j >= 0; j--)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			auto u = double(i) / image_width;
			auto v = double(j) / image_height;
			ray r(origin, lower_left_corner + horizontal * u + vertical * v);
			
			vec3 color = ray_color(r, world);
			color.write_color(std::cout);
		}
	}
	std::cerr << "\nDone.\n";
}