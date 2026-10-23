#ifndef __PPM_RENDER_H__
#define __PPM_RENDER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static const char FILE_NAME[] = "out.ppm";
static const char MAGIC_NUMBER[] = "P6";
enum {
	COLOR_RESOLUTION = 256,
	WIDTH = 1920,
	HEIGHT = 1200,
};

typedef enum {
	RED,
	GREEN,
	BLUE,
	COLOR_ALL,
} color_e;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_d;

void render_image();
void set_pixel(size_t h, size_t w, color_e color, bool set_Nres);
void set_pixel_rgb(size_t h, size_t w, pixel_d color);
void set_region(size_t h1, size_t h2, size_t w1, size_t w2, color_e color, bool set_Nres);

#endif //__PPM_RENDER_H__
