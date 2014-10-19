#ifndef _TEST_H_
#define _TEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "minunit.h"
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_ext.h"
#include "preprocessing.h"
#include "benchmark.h"

int main();
void print_limits();
char* all_tests();
char* test_load_image();
char* test_average_filter();
char* test_greyscale();
char* test_gaussian_blur();
char* test_histogram();
char* test_binarize();
char* test_RLSA();

#endif
