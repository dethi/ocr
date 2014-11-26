#ifndef _STB_IMAGE_EXT_H_
#define _STB_IMAGE_EXT_H_

#include <stdlib.h>
#include <stdio.h>
#include "stb_image.h"
#include "stb_image_write.h"

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct {
    uchar* data;
    int x;
    int y;
    int comp;
} t_img_desc;

static inline
int xytoi(int x, int y, t_img_desc* img) {
    return (img->comp) * (x + (img->x) * y);
}

t_img_desc* load_image(char* filename, int comp);
int write_image(char* filename, t_img_desc* img);
void free_image(t_img_desc* img);
int grey2RGB(t_img_desc* img);
uint* histogram(t_img_desc* img);
uint* histogram_fast(t_img_desc* img);

#endif
