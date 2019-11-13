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

//bool rayTriangleCollision()
//{
//}

bool rayBoxCollision(Ray& ray, Box& box, float& t)
{
	float min = -100000000.f;
	float max = 100000000.0f;

	//f is min, e is max
	Vector3 p = box.centre - ray.origin;

	for (int i = 0; i < 3; i++)
	{
		float e = dot(box.vecArray[i], p);
		//std::cout << "e is  " <<e<< std::endl;
		float f = dot(ray.origin, box.vecArray[i]);
		//std::cout << "abs F\t" <<abs(f)<<std::endl;
		if (abs(f) > 0.0000000001f)
		{

			float t1 = e + box.lengthArray[i] / f;
			float t2 = e - box.lengthArray[i] / f;

				//std::cout << "t1 " << t1 <<std::endl;
			//std::cout << "t2 " << t2 <<std::endl << std::endl;


			if (t1 > t2)
				swap(t1, t2);
			{
				//std::cout << "t1 " << t1 << std::endl;
				//std::cout << "t2 " << t2 << std::endl << std::endl;
				//std::cin.get();
			}
			if (t1 > min)
			{
				min = t1;
				//std::cout << "min " << min << std::endl;
			}
			if (t2 < max)
			{
				max = t2;
				//std::cout << "max " << max << std::endl;
			}

			if (min > max)
			{

				//std::cout << "min was bigger "<<std::endl;
				return false;
			}
			if (max < 0)
			{

				//std::cout << "max was bigger " << std::endl;
				return false;
			}
		}
		else if (-e - box.lengthArray[i] > 0 || -e + box.lengthArray[i] < 0)
			return false;
	}
	//std::cout << "min " << min <<std::endl;
	//std::cout << "max " << max <<std::endl;

	if (min > 0)
	{
		t = min;
		//std::cout << t << std::endl;
		return true;
	}
	else
	{
		t = max;
		//std::cout << t << std::endl;
		return true;
	}
		





	//min = (box.min.x() - )
	/*float ymin = (box.min.y() - ray.origin.y()) / ray.direction.y(); //Y-axis slab
	float ymax = (box.max.y() - ray.origin.y()) / ray.direction.y();
	if (ymin > ymax)
		swap(ymin, ymax);

	float xmin = (box.min.x() - ray.origin.x()) / ray.direction.y(); //Y-axis slab
	float xmax = (box.max.x() - ray.origin.x()) / ray.direction.y();
	if (xmin > xmax)
		swap(xmin, xmax);

	float zmin = (box.min.z() - ray.origin.z()) / ray.direction.z(); //Z-axis slab
	float zmax = (box.max.z() - ray.origin.z()) / ray.direction.z();
	if (zmin > zmax)
		swap(zmin, zmax);

	float tmin = */

	//Vector3 obbCentre = ((box.max - box.min) / 2);
	//Vector3 p = obbCentre - ray.origin;
	//float e = dot(Vector3(x, y, z), p);
	//float f = dot(Vector3(x, y, z), ray.direction);

}

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

	Box box(Vector3(50, 50, 50), Vector3(1, 0, 0), 50.f, Vector3(0, 1, 0), 50.f, Vector3(0, 0, 1), 50.f, Color3(255, 255, 255));//Center, 3 vectors each followed by their length


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
			bool hitBox = rayBoxCollision(ray, box, t);

			if (hitPlane)
			{
				setPixel(image_map, x, y, plane.color);
			}

			if (hitSphere)
			{
				//std::cout << "Hit sphere" << std::endl;
				setPixel(image_map, x, y, sphere.color);
			}

			if (hitBox)
			{

				std::cout << "Hit box" << std::endl;
				setPixel(image_map, x, y, box.color);
			}
		}
	}

	stbi_write_png("output.png", W, H, C_CHANNELS, image_map, W * C_CHANNELS);
	//delete[] image_map;
	return 0;
}

