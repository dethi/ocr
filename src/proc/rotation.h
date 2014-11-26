#ifndef _ROTATION_H_
#define _ROTATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../stb_image/stb_image_ext.h"

typedef struct {
    double rho;
    double theta;
} t_hough;

double hough(t_img_desc *img);

#endif
