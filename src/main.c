#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "ppm_render.h"

#define MAX_ITER_VALUE 32

typedef struct {
	double re;
	double im;
} complex_d;

static size_t mandelbrot_set[HEIGHT][WIDTH];

void square_complex(complex_d *c);
void add_to_complex(complex_d *t, const complex_d *c);
double get_complex_module(const complex_d *c);

void create_mandelbrot_set(size_t width, size_t height, double x_min, double x_max, double y_min, double y_max);


int main(int argc, char **argv) {
	create_mandelbrot_set(WIDTH, HEIGHT, -2.5, 1.0, -1.25, 1.25);
	for(size_t h = 0;h < HEIGHT;h++) {
		for(size_t w = 0;w < WIDTH;w++) {
			pixel_d pixel = {0};
			if(mandelbrot_set[h][w] >= MAX_ITER_VALUE) {
				pixel = (pixel_d){COLOR_RESOLUTION - 1, COLOR_RESOLUTION - 1, COLOR_RESOLUTION - 1};
				// pixel = (pixel_d){COLOR_RESOLUTION / 4 * 3, COLOR_RESOLUTION / 4 * 3, COLOR_RESOLUTION / 4 * 3};
			} else {
				uint8_t color_intens = (uint8_t)(mandelbrot_set[h][w] * COLOR_RESOLUTION / MAX_ITER_VALUE);
				char chosen_color = 'm';
				if (argc > 1) {
					chosen_color = argv[1][0];
				}
				switch(chosen_color) {
					case 'r':
						pixel.red = color_intens;
						break;
					case 'g':
						pixel.green = color_intens;
						break;
					case 'b':
						pixel.blue = color_intens;
						break;
					case 'm':
						pixel.red = color_intens;
						pixel.blue = color_intens;
						break;
					case 'y':
						pixel.red = color_intens;
						pixel.green = color_intens;
						break;
					case 'c':
						pixel.green = color_intens;
						pixel.blue = color_intens;
						break;
					case 'w':
						pixel.red = color_intens;
						pixel.green = color_intens;
						pixel.blue = color_intens;
						break;
					default:
						pixel.red = color_intens;
						pixel.blue = color_intens;
				}
			}
			set_pixel_rgb(h, w, pixel);
		}
	}
	render_image();


	return 0;
}

void square_complex(complex_d *c) {
	double buff = c->re * c->re - c->im * c->im;
	c->im = 2 * c->re * c->im;
	c->re = buff;
}
void add_to_complex(complex_d *t, const complex_d *c) {
	t->re += c->re;
	t->im += c->im;
}
double get_complex_module(const complex_d *c) {
	return sqrt(c->re * c->re + c->im * c->im);
}

void create_mandelbrot_set(size_t width, size_t height, double x_min, double x_max, double y_min, double y_max) {
	for(size_t h = 0;h < height;h++) {
		for(size_t w = 0;w < width;w++) {
			double x = (double)w / (double)width * (x_max - x_min) + x_min;
			double y = (double)h / (double)height * (y_max - y_min) + y_min;
			complex_d C = {x,y};
			complex_d Z = C;
			size_t i;
			bool diverged = false;
			for(i = 0;i < MAX_ITER_VALUE && diverged == false;i++) {
				square_complex(&Z);
				add_to_complex(&Z, &C);
				diverged = get_complex_module(&Z) > 2.0;
			}
			mandelbrot_set[h][w] = i;
		}
	}
}

