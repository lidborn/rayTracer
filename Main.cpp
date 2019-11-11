#include "Vector3.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int W = 256;
const int H = 256;
const int C_CHANNELS = 3;

void setPixel(int* image_map, int x, int y, Vector3 rgb)
{
	image_map[x * y * C_CHANNELS + 0] = rgb[0];
	image_map[x * y * C_CHANNELS + 1] = rgb[1];
	image_map[x * y * C_CHANNELS + 2] = rgb[2];
};

struct Ray
{
	Vector3 origin;
	Vector3 direction;
	double t = 0;
};

struct Plane
{
	Vector3 normal;
	double d = 0;
};

int main()
{
	int* image_map = new int[W * H * C_CHANNELS];

	stbi_write_png("output.png", W, H, C_CHANNELS, image_map, W * C_CHANNELS);
	delete[] image_map;
	return 0;
}

