#include<iostream>

#include "rtweekend.h"

#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::make_shared;

vec3 ray_color(const ray& r,const hittable_list& world,int depth)
{
	if (depth <= 0)
		return vec3(0, 0, 0);

	hit_record record;
	if (!world.hit(r, 0.001, infinity, record))
		return vec3(0,0,0);
		//return background;		

	vec3 atten;
	ray scattered;
	vec3 emitted = record.mat_ptr->emitted(record.u, record.v, record.point);
	if (!record.mat_ptr->scatter(r, record, atten, scattered))
		return emitted;

	return emitted + atten * ray_color(scattered, world, depth - 1);
}

hittable_list two_spheres()
{
	hittable_list objects;
	auto checker = make_shared<checker_texture>(make_shared<constant_texture>(vec3(0.2, 0.3, 0.1)),
												make_shared<constant_texture>(vec3(0.9, 0.9, 0.9)));
	objects.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	objects.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));
	return objects;
}

hittable_list random_scene()
{
	hittable_list world;

	auto checker = make_shared<checker_texture>(make_shared<constant_texture>(vec3(0.2, 0.3, 0.1)),
												make_shared<constant_texture>(vec3(0.9, 0.9, 0.9)));

	world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

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
					//world.add(
					//	make_shared<moving_sphere>(center, 
					//		center + vec3(0, random_double(0, 0.5), 0), 0.0, 1.0,
					//		0.2, make_shared<lambertian>(albedo)));
					//world.add(
					//	make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo))
					//);
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

	//world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

	world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return static_cast<hittable_list>(make_shared<bvh_node>(world, 0, 1));
	return world;
}

hittable_list earth()
{
	int nx, ny, nn;
	unsigned char* texture_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	shared_ptr<lambertian> earth_surface = make_shared<lambertian>(make_shared<image_texture>(texture_data, nx, ny));
	shared_ptr<hittable> globe = make_shared<sphere>(vec3(0, 0, 0), 2, earth_surface);
	return hittable_list(globe);
}

hittable_list simple_light()
{
	hittable_list objects;
	auto checker = make_shared<checker_texture>(make_shared<constant_texture>(vec3(0.2, 0.3, 0.1)),
												make_shared<constant_texture>(vec3(0.9, 0.9, 0.9)));
	objects.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));
	objects.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(checker)));

	shared_ptr<diffuse_light> difflight = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(4, 4, 4)));
	objects.add(make_shared<sphere>(vec3(0, 7, 0), 2, difflight));
	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}

hittable_list cornell_box()
{
	hittable_list objects;

	shared_ptr<lambertian> red = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.65, 0.05, 0.05)));
	shared_ptr<lambertian> white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	shared_ptr<lambertian> green = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.12, 0.45, 0.15)));
	shared_ptr<diffuse_light> light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(15, 15, 15)));

	//??
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	//??
	objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	//????????
	objects.add(make_shared<box>(vec3(130, 0, 65), vec3(295, 165, 230), white));
	objects.add(make_shared<box>(vec3(265, 0, 295), vec3(430, 330, 460), white));

	return objects;
}

hittable_list final_scene()
{
	hittable_list boxes1;
	shared_ptr<material> ground = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.48, 0.83, 0.53)));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++)
	{
		for (int j = 0; j < boxes_per_side; j++)
		{
			double w = 100.0;
			double x0 = -1000.0 + i * w;
			double z0 = -1000.0 + j * w;
			double y0 = 0.0;
			double x1 = x0 + w;
			double y1 = random_double(1, 101);
			double z1 = z0 + w;

			boxes1.add(make_shared<box>(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
		}
	}

	hittable_list objects;
	objects.add(make_shared<bvh_node>(boxes1, 0, 1));

	shared_ptr<material> light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(7, 7, 7)));
	objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	objects.add(make_shared<sphere>(vec3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	objects.add(make_shared<sphere>(vec3(0, 150, 145), 50, make_shared<metal>(vec3(0.8, 0.8, 0.9), 10.0)));

	hittable_list boxes2;
	shared_ptr<material> white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxes2.add(make_shared<sphere>(vec3::random(0, 165), 10, white));
	}
	objects.add(make_shared<translate>(make_shared<bvh_node>(boxes2, 0.0, 1.0), vec3(-100, 270, 395)));

	return objects;
}

void main()
{
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 600;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samplers_per_pixel = 100;
	const int max_depth = 50;

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	
	//camera
	vec3 lookfrom(478, 278, -600);
	vec3 lookat(278, 278, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10;
	auto aperture = 0.0;
	hittable_list world = final_scene();
	camera cam(lookfrom, lookat, vup, 40, double(image_width) / image_height, aperture, dist_to_focus);

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

