#include "Vector3.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

const int W = 256;
const int H = 256;
const int C_CHANNELS = 3;

struct Color3
{
	uint8_t color[3];

	uint8_t operator[](int index)
	{
		return this->color[index];
	}
	Color3(uint8_t a, uint8_t b, uint8_t c)
	{
		color[0] = a;
		color[1] = b;
		color[2] = c;
	}
	Color3()
	{
		color[0] = (uint8_t)0;
		color[1] = (uint8_t)0;
		color[2] = (uint8_t)0;
	}
};

void setPixel(uint8_t* image_map, int x, int y, Color3 rgb)
{
	int pixelRGB = (W * y + x) * C_CHANNELS;
	//int pixelRGB = (int)x * (int)y * C_CHANNELS;
	image_map[pixelRGB + 0] = rgb.color[0];
	image_map[pixelRGB + 1] = rgb.color[1];
	image_map[pixelRGB + 2] = rgb.color[2];
};

struct Ray
{
	Vector3 origin;
	Vector3 direction;
	float t = 0;
};

struct Plane
{
	Vector3 normal;
	float distance = 0;
	Color3 color;
};

struct Sphere
{
	Vector3 centre;
	float radius = 1;
	Color3 color;
};

bool rayPlaneCollision(Ray & ray, Plane & plane, float& t)
{
	float n_dot_d = dot(plane.normal, ray.direction); //normal dot direction

	if (n_dot_d == 0.0) //Can not divide by zero.
		return false;

	float n_dot_o = dot(plane.normal, ray.origin);
	t = (plane.distance - n_dot_o) / n_dot_d; //Minus framför plane.distance?

	return true;
};

template<class num>
num min(num a, num b)
{
	if (a < b)
		return a;
	else
		return b;
};

bool raySphereCollision(Ray & ray, Sphere & sphere, float& t)
{
	float b = dot(ray.direction, (ray.origin - sphere.centre));
	float c = dot((ray.origin - sphere.centre), (ray.origin - sphere.centre)) - pow(sphere.radius, 2);

	float t1 = 0;
	float t2 = 0;

	if ((pow(b, 2) - c) > 0)
	{
		t1 = -b + sqrt(pow(b, 2) - c);
		t2 = -b - sqrt(pow(b, 2) - c);
		//std::cout << "T1: " << t1 << " T2: " << t2 << std::endl;
	}
	else
		return false;
	
	t = min(t1, t2);
	//std::cout << t << std::endl;

	if (t != 0)
		return true;
	else
		return false;
};

int main()
{
	uint8_t image_map[W * H * C_CHANNELS]{ 0 };// = new uint8_t[W * H * C_CHANNELS]{ 0 };

	Plane plane;
	plane.distance = -100;
	plane.normal = Vector3(0, 0, -1);
	plane.color = Color3(0, 100, 100);

	Sphere sphere;
	sphere.radius = 60;
	sphere.centre = Vector3(50, 100, 45);
	sphere.color = Color3(255, 0, 0);

	//setPixel(image_map, 0, 0, Color3(255,50,255));

	for (int y = 0; y < H; y++)
	{
		for (int x = 0; x < W; x++)
		{
			Ray ray;
			ray.direction = Vector3(0, 0, 1);
			ray.origin = Vector3(x, y, 0);
			float t = 0;
			bool hitPlane = rayPlaneCollision(ray, plane, t);
			bool hitSphere = raySphereCollision(ray, sphere, t);

			if (hitPlane)
			{
				setPixel(image_map, x, y, plane.color);
			}

			if (hitSphere)
			{
				//std::cout << "Hit sphere" << std::endl;
				setPixel(image_map, x, y, sphere.color);
			}
		}
	}

	stbi_write_png("output.png", W, H, C_CHANNELS, image_map, W * C_CHANNELS);
	//delete[] image_map;
	return 0;
}

