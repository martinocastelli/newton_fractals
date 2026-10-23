#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "ppm_render.h"

static pixel_d data[HEIGHT][WIDTH] = {};

void render_image() {
	FILE *fp = fopen(FILE_NAME, "wb"); // binary mode
	assert(fp != NULL);
	fprintf(fp, "%s\n%d %d\n%d\n", MAGIC_NUMBER, WIDTH, HEIGHT, COLOR_RESOLUTION - 1);

	for(size_t h = 0;h < HEIGHT;h++) {
		for(size_t w = 0;w < WIDTH; w++) {
			fwrite((char *)(data[h] + w), sizeof(pixel_d), 1, fp);
		}
	}
	fclose(fp);
}

void set_pixel_rgb(size_t h, size_t w, pixel_d color) {
	assert(w < WIDTH);
	assert(h < HEIGHT);
	data[h][w] = color;
}

void set_pixel(size_t h, size_t w, color_e color, bool set_Nres) {
	assert(w < WIDTH);
	assert(h < HEIGHT);
	uint8_t clr_code = set_Nres?COLOR_RESOLUTION - 1:0;
	switch(color) {
		case RED:
			data[h][w].red = clr_code;
			break;
		case GREEN:
			data[h][w].green = clr_code;
			break;
		case BLUE:
			data[h][w].blue = clr_code;
			break;
		case COLOR_ALL:
			data[h][w].red = clr_code;
			data[h][w].green = clr_code;
			data[h][w].blue = clr_code;

	}
}

void set_region(size_t h1, size_t h2, size_t w1, size_t w2, color_e color, bool set_Nres) {
	for(size_t h = h1; h < h2;h++) {
		for(size_t w = w1;w < w2;w++) {
			set_pixel(h, w, color, set_Nres);
		}
	}
}
