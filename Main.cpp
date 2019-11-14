//Jakob Lidborn, Christian Leo, Ghazi Hicheri | 2019-11-14
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

struct Triangle
{
	Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normal, Color3 color)
	{
		pointA = a; 
		pointB = b;
		pointC = c;

		this->normal = normal;
		this->color = color;
	}

	Triangle() {};

	Vector3 pointA, pointB, pointC;
	Vector3 normal;
	Color3 color;
};

struct Box
{
	Box(Vector3 centre, Vector3 u, float hu, Vector3 v, float hv, Vector3 w, float hw, Color3 color)
	{
		this->centre = centre;

		this->u = u;
		this->hu = hu;

		this->v = v;
		this->hv = hv;

		this->w = w;
		this->hw = hw;

		this->color = color;

		vecArray[0] = u;
		vecArray[1] = v;
		vecArray[2] = w;

		lengthArray[0] = hu;
		lengthArray[1] = hv;
		lengthArray[2] = hw;
	}

	Box() {};

	Vector3 centre;

	Vector3 u;
	float hu; //Half of the box's length in u's direction

	Vector3 v;
	float hv; //Half of the box's length in v's direction

	Vector3 w;
	float hw; //Half of the box's length in w's direction

	Vector3 vecArray[3];
	float lengthArray[3];

	Color3 color;
};

template<class num>
num min(num a, num b)
{
	if (a < b)
		return a;
	else
		return b;
};

template<class num>
num max(num a, num b)
{
	if (a > b)
		return a;
	else
		return b;
};

template<class num>
void swap(num& a, num& b)
{
	num temp = b;
	b = a;
	a = temp;
};

bool rayPlaneCollision(Ray& ray, Plane& plane, float& t)
{
	float n_dot_d = dot(plane.normal, ray.direction); //normal dot direction

	if (n_dot_d == 0.0) //Can not divide by zero.
		return false;

	float n_dot_o = dot(plane.normal, ray.origin);
	t = (plane.distance - n_dot_o) / n_dot_d; //Minus framför plane.distance?

	return true;
};

bool raySphereCollision(Ray& ray, Sphere& sphere, float& t)
{
	float b = dot(ray.direction, (ray.origin - sphere.centre));
	float c = dot((ray.origin - sphere.centre), (ray.origin - sphere.centre)) - pow(sphere.radius, 2);

	float t1 = 0;
	float t2 = 0;

	if ((pow(b, 2) - c) > 0)
	{
		t1 = -b + sqrt(pow(b, 2) - c);
		t2 = -b - sqrt(pow(b, 2) - c);
	}
	else
		return false;

	t = min(t1, t2);

	if (t != 0)
		return true;
	else
		return false;
};

bool rayTriangleCollision(Ray& ray, Triangle& triangle, float& t)
{
	//Transform into unit triangle at origin for testing.
	//We might want to return u and w later

	float epsilon = 0.000000001f;
	Vector3 e1 = triangle.pointB - triangle.pointA; //Edge 1
	Vector3 e2 = triangle.pointC - triangle.pointA; //Edge 2
	Vector3 q = cross(ray.direction, e2); //note to self for debugging: could try triangle's direction
	float a = dot(e1, q);
	if (a > -epsilon && a < epsilon) //If close to 0, then floating point error for 0.
		return false;
	float f = 1 / a;
	Vector3 s = ray.origin-triangle.pointA;	float u = f * (dot(s, q)); //Don't need this for now.
	if (u < 0.0f)
		return false;
	Vector3 r = cross(s, e1);
	float v = f*(dot(ray.direction, r)); //Don't need this for now.
	if (v < 0.0f || u + v > 1.0f)
		return false;
	t = f * dot(e2, r);
	return true;

}

bool rayBoxCollision(Ray& ray, Box& box, float& t)
{

	float min = -100000000.f;
	float max = 100000000.0f;

	Vector3 p = box.centre - ray.origin;

	//Check all 3 slabs
	for (int i = 0; i < 3; i++)
	{
		float e = dot(box.vecArray[i], p);
		float f = dot(box.vecArray[i],ray.direction );
		if (abs(f) > 0.0000000001f)
		{

			float t1 = e + box.lengthArray[i] / f;
			float t2 = e - box.lengthArray[i] / f;

			if (t1 > t2)
			{
				swap(t1, t2);
			}
			if (t1 > min)
			{
				min = t1;
			}
			if (t2 < max)
			{
				max = t2;
			}

			if (min > max)
			{
				return false;
			}
			if (max < 0)
			{
				return false;
			}
		}
		else if (-e - box.lengthArray[i] > 0 || -e + box.lengthArray[i] < 0)
			return false;
	}

	if (min > 0)
	{
		t = min;
		return true;
	}
	else
	{
		t = max;
		return true;
	}
}

int main()
{
	uint8_t image_map[W * H * C_CHANNELS]{ 0 };

	Plane plane;
	plane.distance = -100;
	plane.normal = Vector3(0, 0, -1);
	plane.color = Color3(0, 100, 100);

	Sphere sphere;
	sphere.radius = 50;
	sphere.centre = Vector3(80, 100, 25);
	sphere.color = Color3(255, 0, 0);

	Triangle triangle;
	triangle.pointA = Vector3(W, 0, 25);
	triangle.pointB = Vector3(W, H, 25);
	triangle.pointC = Vector3(50, 0, 25);
	triangle.color = Color3(100, 37, 90);

	Box box(Vector3(70.f, 70.f, 25.f), Vector3(1, 0, 0), 50.f, Vector3(0, 1, 0), 50.f, Vector3(0, 0, 1), 50.f, Color3(255, 255, 255));//Center, 3 vectors each followed by their length


	//setPixel(image_map, 0, 0, Color3(255,50,255));

	for (int y = 0; y < H; y++)
	{
		for (int x = 0; x < W; x++)
		{
			Ray ray;
			ray.direction = Vector3(0, 0, 1);
			ray.origin = Vector3(x, y, 0);
			float t = 10000;
			float currentDepth = 0;
			bool hitPlane = rayPlaneCollision(ray, plane, currentDepth);
			if (currentDepth < t && t > 0) //Check it closest and in front of camera, otherwise we might as well pretend we didn't hit it.
			{
				t = currentDepth;
			}
			else
				hitPlane = false;

			bool hitSphere = raySphereCollision(ray, sphere, currentDepth);
			if (currentDepth < t && t > 0) //Check it closest and in front of camera, otherwise we might as well pretend we didn't hit it.
			{
				t = currentDepth;
			}
			else
				hitSphere = false;

			bool hitBox = rayBoxCollision(ray, box, currentDepth);
			if (currentDepth < t && t > 0) //Check it closest and in front of camera, otherwise we might as well pretend we didn't hit it.
			{
				t = currentDepth;
			}
			else
				hitBox = false;

			bool hitTriangle = rayTriangleCollision(ray, triangle, currentDepth);
			if (currentDepth < t && t > 0) //Check it closest and in front of camera, otherwise we might as well pretend we didn't hit it.
			{
				t = currentDepth;
			}
			else
				hitTriangle = false;
			
			//------------------------------------------------------ Drawing Below	
			if (hitPlane)
			{
				setPixel(image_map, x, y, plane.color);
			}

			if (hitSphere)
			{
				setPixel(image_map, x, y, sphere.color);
			}

			if (hitBox)
			{
				setPixel(image_map, x, y, box.color);
			}

			if (hitTriangle)
			{
				setPixel(image_map, x, y, triangle.color);
			}
		}
	}

	stbi_write_png("Jakob Lidborn, Christian Leo, Ghazi Hicheri - 2019-11-14.png", W, H, C_CHANNELS, image_map, W * C_CHANNELS);
	return 0;
}

