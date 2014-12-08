#ifndef _DETECTION_H_
#define _DETECTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../stb_image/stb_image_ext.h"

typedef struct {
    int *element;
    int length;
} vect;

typedef struct {
    int x_min;
    int x_max;
    int y_min;
    int y_max;
} coor;

coor *detect(t_img_desc * img, size_t * length);
t_img_desc *get_data(t_img_desc * img, coor c);

#endif
