#ifndef _ROTATION_H_
#define _ROTATION_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../stb_image/stb_image_ext.h"

typedef struct {
    double rho;
    double theta;
} t_hough;

/* Return the rotation angle in radians */
double hough(t_img_desc *img);

/* Take an angle in radians */
void rotate(t_img_desc *img, double theta);

/* Automatic function, return the rotation applied in degrees */
double rotate_img(t_img_desc *img);

#endif
