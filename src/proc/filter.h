#ifndef _FILTER_H_
#define _FILTER_H_

#include <stdlib.h>
#include <stdio.h>
#include "../stb_image/stb_image_ext.h"

void filter_mask(t_img_desc* img, const char *mask, int sum_mask, int n);

#endif