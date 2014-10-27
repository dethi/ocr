#ifndef _PREPROCESSING_H_
#define _PREPROCESSING_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "../stb_image/stb_image_ext.h"

void grey_scale(t_img_desc* tab);
void binarize_basic(t_img_desc* img);
void binarize_otsu(t_img_desc* img);

#endif
