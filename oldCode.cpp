/*/#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
//HEADER  P6 W H
//Body -- data
typedef struct vec3_t
{
	float x, y, z;

}vec3_t;

typedef struct ray_t
{
	vec3_t origin;
	vec3_t direction;
}ray_t;
typedef struct plane_t
{
	vec3_t normal;
	float d;
}plane_t;
float dot_vec3(vec3_t a, vec3_t b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
bool ray_plane_test(ray_t ray, plane_t plane, float* t)
{
	float n_dot_d = dot_vec3(plane.normal, ray.direction);
	if (n_dot_d == 0.0f) { return false; }
	float n_dot_o = dot_vec3(plane.normal, ray.origin);
	*t = (plane.d - n_dot_o) / n_dot_d;
	return true;
}
void write_ppm(const char* filename, int w, int h, int n_channels, const uint8_t* image_ptr)
{
	FILE* f_ptr;
	errno_t err = fopen_s(&f_ptr, filename, "wb");
	fprintf(f_ptr, "P6\n%i%i \n255\n", w, h);
	fwrite(image_ptr, 1, w * h * n_channels, f_ptr);
	fclose(f_ptr);
}

void set_pixel(uint8_t* image_ptr, int x, int y, int w, uint8_t r, uint8_t g, uint8_t b, int n_channels)
{
	int idx = (w * y + x) * n_channels;
	image_ptr[idx] = r;
	image_ptr[idx + 1] = g;
	image_ptr[idx + 2] = b;
}
int main()
{
	int w = 256, h = 256, n_channel = 3; //RGB IMG
	uint8_t* image_ptr = new uint8_t(w * h * n_channel);
	plane_t plane;
	plane.normal = vec3_t{ 0, 0, -1 };
	plane.d = -100;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			ray_t ray;
			ray.origin = vec3_t{ x, y, 0 };
			ray.direction = vec3_t{ 0,0,1 };
			float t = 0.0f;
			bool hit = ray_plane_test(ray, plane, &t);
			if (hit)
			{
				set_pixel(image_ptr, x, y, w, n_channel, 0x00, 0xFF, 0xFF);
			}
		}
	set_pixel(image_ptr, 0, 0, w, n_channel, 255, 0, 0);
	write_ppm("output.ppm", w, h, n_channel, image_ptr);

	//stb_write_png("output.png", w, h, n_channel, image_ptr, w * n_channel);
	return 0;
}*/