#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "ppm_render.h"

static rgb_d data[HEIGHT][WIDTH] = {};



static rgb_d hsv_to_rgb(hsv_d hsv);

void ppm_render_image(const char *file_name) {
	FILE *fp;
	if(file_name != NULL) {
		fp = fopen(file_name, "wb"); 
	} else {
		fp = fopen(DEFAULT_FILE_NAME, "wb");
	}
	assert(fp != NULL);
	fprintf(fp, "%s\n%d %d\n%d\n", MAGIC_NUMBER, WIDTH, HEIGHT, COLOR_RESOLUTION - 1);

	for(size_t h = 0;h < HEIGHT;h++) {
		for(size_t w = 0;w < WIDTH; w++) {
			fwrite((char *)(data[h] + w), sizeof(rgb_d), 1, fp);
		}
	}
	fclose(fp);
}

void ppm_set_pixel_color(size_t h, size_t w, color_e color, bool set_Nres) {
	assert(w < WIDTH);
	assert(h < HEIGHT);
	uint8_t clr_code = set_Nres?COLOR_RESOLUTION - 1:0;
	switch(color) {
		case RED:
			data[h][w].r = clr_code;
			break;
		case GREEN:
			data[h][w].g = clr_code;
			break;
		case BLUE:
			data[h][w].b = clr_code;
			break;
		case MAGENTA:
			data[h][w].r = clr_code;
			data[h][w].b = clr_code;
			break;
		case CYAN:
			data[h][w].r = clr_code;
			data[h][w].b = clr_code;
			break;
		case YELLOW:
			data[h][w].r = clr_code;
			data[h][w].b = clr_code;
			break;
		case WHITE:
			data[h][w].r = clr_code;
			data[h][w].g = clr_code;
			data[h][w].b = clr_code;
	}
}
void ppm_set_pixel_rgb(size_t h, size_t w, rgb_d rgb) {
	assert(w < WIDTH);
	assert(h < HEIGHT);
	data[h][w] = rgb;
}
void ppm_set_pixel_hsv(size_t h, size_t w, hsv_d hsv) {
	ppm_set_pixel_rgb(h, w, hsv_to_rgb(hsv));
}
void ppm_set_region_color(size_t h1, size_t h2, size_t w1, size_t w2, color_e color, bool set_Nres) {
	for(size_t h = h1; h < h2;h++) {
		for(size_t w = w1;w < w2;w++) {
			ppm_set_pixel_color(h, w, color, set_Nres);
		}
	}
}
void ppm_set_region_rgb(size_t h1, size_t h2, size_t w1, size_t w2, rgb_d rgb) {
	for(size_t h = h1; h < h2;h++) {
		for(size_t w = w1;w < w2;w++) {
			ppm_set_pixel_rgb(h, w, rgb);
		}
	}
}
void ppm_set_region_hsv(size_t h1, size_t h2, size_t w1, size_t w2, hsv_d hsv) {
	ppm_set_region_rgb(h1, h2, w1, w2, hsv_to_rgb(hsv));
}

static rgb_d hsv_to_rgb(hsv_d hsv) {
	double h, s, v, r, g, b;
	h = (double)hsv.h / 256.0 * 360;
	s = (double)hsv.s / 256.0;
	v = (double)hsv.v / 256.0;
	double c = v * s;
	double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
	double m = v - c;

	if (h >= 0.0 && h < 60.0) {
		r = c; g = x; b = 0.0;
	} else if (h >= 60.0 && h < 120.0) {
		r = x; g = c; b = 0.0;
	} else if (h >= 120.0 && h < 180.0) {
		r = 0.0; g = c; b = x;
	} else if (h >= 180.0 && h < 240.0) {
		r = 0.0; g = x; b = c;
	} else if (h >= 240.0 && h < 300.0) {
		r = x; g = 0.0; b = c;
	} else if (h >= 300.0 && h < 360.0) {
		r = c; g = 0.0; b = x;
	} else {
		fprintf(stderr, "impossible situation returning\n");
		exit(1);
	}

	r = round((r + m) * 255.0);
	g = round((g + m) * 255.0);
	b = round((b + m) * 255.0);

	return (rgb_d){(uint8_t)r, (uint8_t)g, (uint8_t)b};
}
