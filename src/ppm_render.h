#ifndef __PPM_RENDER_H__
#define __PPM_RENDER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static const char DEFAULT_FILE_NAME[] = "out.ppm";
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
	MAGENTA,
	YELLOW,
	CYAN,
	WHITE,

	COLOR_MAX,
	COLOR_FIRST = RED
} color_e;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_d;

typedef struct {
	uint8_t h;
	uint8_t s;
	uint8_t v;
} hsv_d;


void ppm_render_image(const char *file_name);
void ppm_set_pixel_color(size_t h, size_t w, color_e color, bool set_Nres);
void ppm_set_pixel_rgb(size_t h, size_t w, rgb_d rgb);
void ppm_set_pixel_hsv(size_t h, size_t w, hsv_d hsv);
void ppm_set_region_color(size_t h1, size_t h2, size_t w1, size_t w2, color_e color, bool set_Nres);
void ppm_set_region_rgb(size_t h1, size_t h2, size_t w1, size_t w2, rgb_d rgb);
void ppm_set_region_hsv(size_t h1, size_t h2, size_t w1, size_t w2, hsv_d hsv);

#endif //__PPM_RENDER_H__
