#ifndef HEADER_OTSU_H
#define HEADER_OTSU_H

typedef struct {
    uchar* data;
    int x;
    int y;
    int comp;
} t_img_desc;

void binarize(t_image_desc*);
t_img_desc* otsu(t_img_desc*);
