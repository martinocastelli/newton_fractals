#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "ppm_render.h"

#define MAX_ITERATION 256
#define MIN_PRECISION 0.01

typedef struct {
	double re;
	double im;
} complex_d;

const complex_d Z1 = {1.0, 0.0};
const complex_d Z2 = {-0.5, 0.866};
const complex_d Z3 = {-0.5, -0.866};

complex_d f(complex_d x);
complex_d df(complex_d x);

complex_d multiply_complex(complex_d z1, complex_d z2);
complex_d divide_complex(complex_d z1, complex_d z2);
complex_d add_complex(complex_d z1, complex_d z2);
complex_d sub_complex(complex_d z1, complex_d z2);
double get_complex_module(complex_d c);
complex_d get_zero(complex_d start);

int main(int argc, char **argv) {
	srand(time(NULL));

	for(size_t h = 0;h < HEIGHT;h++) {
		for(size_t w = 0;w < WIDTH;w++) {
			complex_d z1 = {(double)w / (double)WIDTH * 4.0 - 2.0, (double)h / (double)HEIGHT * 4.0 - 2.0};
			complex_d z2 = get_zero(z1);
			color_e col;
			if(get_complex_module(sub_complex(z2, Z1)) < MIN_PRECISION) {
				col = RED;
			} else if (get_complex_module(sub_complex(z2, Z2)) < MIN_PRECISION) {
				col = GREEN;
			} else if (get_complex_module(sub_complex(z2, Z3)) < MIN_PRECISION) {
				col = BLUE;
			} else {
				col = COLOR_ALL;
			}
			set_pixel(h, w, col, true);
		}
	}
	render_image();
	return 0;
}
complex_d f(complex_d x) {
	return add_complex(multiply_complex(x, multiply_complex(x, x)), (complex_d){-1,0});
}
complex_d df(complex_d x) {
	return multiply_complex(multiply_complex(x, x), (complex_d){3,0});
}

complex_d multiply_complex(complex_d z1, complex_d z2) {
	return (complex_d){z1.re * z2.re - z1.im * z2.im, z1.re * z2.im + z1.im * z2.re};
}
complex_d divide_complex(complex_d z1, complex_d z2) {
	double buff = z2.re * z2.re + z2.im * z2.im;
	return (complex_d){(z1.re * z2.re + z1.im * z2.im) / buff, (z1.im * z2.re - z1.re * z2.im) / buff};
}
complex_d add_complex(complex_d z1, complex_d z2) {
	return (complex_d){z1.re + z2.re, z1.im + z2.im};
}
complex_d sub_complex(complex_d z1, complex_d z2) {
	return (complex_d){z1.re - z2.re, z1.im - z2.im};
}
double get_complex_module(complex_d c) {
	return sqrt(c.re * c.re + c.im * c.im);
}
complex_d get_zero(complex_d start) {
	complex_d z = start;

	size_t i;
	for(i = 0; i < MAX_ITERATION && get_complex_module(f(z)) > MIN_PRECISION;i++){
		z = sub_complex(z, divide_complex(f(z), df(z)));
	}
	if(i >= MAX_ITERATION) {
		fprintf(stderr, "MAX_ITERATION reached for %lf + i%lf reached %lf + i%lf with delta = %lf\n", start.re, start.im, z.re, z.im, get_complex_module(z));
	}
	return z;
}


