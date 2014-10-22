#ifndef _PREPROCESSING_H_
#define _PREPROCESSING_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "../stb_image/stb_image_ext.h"

void grey_scale(t_img_desc* tab);
uchar human_grey(uchar r, uchar g, uchar b);
void binarize(uchar* data, int size, int th);
void binarize_basic(t_img_desc* img);
void binarize_otsu(t_img_desc* img);
int thresold(uint* h, size_t size);
void average_filter(t_img_desc* img);
void gaussian_blur(t_img_desc* img, uchar *mask, int sum_mask, int n);

#endif
