#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "ppm_render.h"

#define MAX_ITERATION 256
#define MIN_PRECISION 0.001

typedef struct {
	double re;
	double im;
} complex_d;


complex_d f(complex_d x);
complex_d df(complex_d x);

complex_d multiply_complex(complex_d z1, complex_d z2);
complex_d divide_complex(complex_d z1, complex_d z2);
complex_d add_complex(complex_d z1, complex_d z2);
complex_d sub_complex(complex_d z1, complex_d z2);
double get_complex_module(complex_d c);
complex_d get_zero(complex_d start);
size_t get_iter_to_zero(complex_d start);
size_t get_which_zero(complex_d z);

int main(int argc, char **argv) {
	srand(time(NULL));

	for(size_t h = 0;h < HEIGHT;h++) {
		for(size_t w = 0;w < WIDTH;w++) {
			complex_d z1 = {(double)w / (double)WIDTH * 4.0 - 2.0, (double)h / (double)HEIGHT * 4.0 - 2.0};
			complex_d z2 = get_zero(z1);
			color_e col = get_which_zero(z2) % COLOR_MAX;
			ppm_set_pixel_color(h, w, col, true);

			// size_t iter = get_iter_to_zero(z1);
			// color_e col = iter % COLOR_MAX;
			// ppm_set_pixel_color(h, w, col, true);
		}
	}
	ppm_render_image(NULL);
	return 0;

}
//z^3 - z
complex_d f(complex_d z) {
	complex_d z2 = multiply_complex(z, z);
	complex_d z3 = multiply_complex(z2, z);
	// complex_d z5 = multiply_complex(z4, z);
	z = sub_complex(z3, z);
	// z = sub_complex(z, (complex_d){1,0});
	return z;
}
//3z^2 - 1
complex_d df(complex_d z) {
	complex_d z2 = multiply_complex(z, z);
	// complex_d z3 = multiply_complex(z2, z);
	z = multiply_complex(z2, (complex_d){3,0});
	z = sub_complex(z, (complex_d){1,0});
	return z;
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
		if(isnan(z.re) || isnan(z.im)) {
			z.re = MIN_PRECISION / 2;
			z.im = MIN_PRECISION / 2;
		}
	}
	if(i >= MAX_ITERATION) {
		fprintf(stderr, "MAX_ITERATION reached for %lf + i%lf reached %lf + i%lf with delta = %lf\n", start.re, start.im, z.re, z.im, get_complex_module(z));
	}
	return z;
}

size_t get_iter_to_zero(complex_d start) {
	complex_d z = start;

	size_t i;
	for(i = 0; i < MAX_ITERATION && get_complex_module(f(z)) > MIN_PRECISION;i++){
		z = sub_complex(z, divide_complex(f(z), df(z)));
	}
	if(i >= MAX_ITERATION) {
		fprintf(stderr, "MAX_ITERATION reached for %lf + i%lf reached %lf + i%lf with delta = %lf\n", start.re, start.im, z.re, z.im, get_complex_module(z));
		i = 0;
	}
	return i;
}
size_t get_which_zero(complex_d z) {
	static complex_d zero_list[10] = {0};
	static size_t zero_list_index = 0;
	size_t i = 0;
	bool found = false;
	while(i < zero_list_index && found == false) {
		if(get_complex_module(sub_complex(zero_list[i], z)) < 2*MIN_PRECISION) {
			found = true;
		} else {
			i++;
		}
	}
	if(i >= zero_list_index) {
		fprintf(stderr, "found new zero: %lf + i%lf\n", z.re, z.im);
		zero_list[zero_list_index++] = z;
	}
	return i;
}
