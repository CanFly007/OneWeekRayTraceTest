#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

class texture
{
public:
	virtual vec3 value(double u, double v, const vec3& p)const = 0;
};

class constant_texture :public texture
{
public:
	constant_texture() {}
	constant_texture(vec3 c) :color(c) {}

	virtual vec3 value(double u, double v, const vec3& p)const
	{
		return color;
	}

public:
	vec3 color;
};

class checker_texture :public texture
{
public:
	checker_texture() {}
	checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1) :odd(t0), even(t1) {}

	virtual vec3 value(double u, double v, const vec3& p)const
	{
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines > 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}

public:
	shared_ptr<texture> odd;
	shared_ptr<texture> even;
};

class image_texture :public texture
{
public:
	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B) :data(pixels), nx(A), ny(B) {}
	~image_texture()
	{
		delete data;
	}

	virtual vec3 value(double u, double v, const vec3& p)const
	{
		if (data == nullptr)
			return vec3(1, 0, 1);

		int i = static_cast<int>(u * nx);
		int j = static_cast<int>((1 - v) * ny - 0.001);

		//clamp(i, 0, nx - 1);
		//clamp(j, 0, ny - 1);
		if (i < 0) i = 0;
		if (j < 0) j = 0;
		if (i > nx - 1) i = nx - 1;
		if (j > ny - 1) j = ny - 1;

		int r = static_cast<int>(data[3 * (i + nx * j) + 0]) / 255.0;
		int g = static_cast<int>(data[3 * (i + nx * j) + 1]) / 255.0;
		int b = static_cast<int>(data[3 * (i + nx * j) + 2]) / 255.0;
		return vec3(r, g, b);
	}

public:
	unsigned char* data;
	int nx, ny;
};

#endif // !TEXTURE_H
