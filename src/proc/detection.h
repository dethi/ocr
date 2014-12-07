#ifndef _DETECTION_H_
#define _DETECTION_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../stb_image/stb_image_ext.h"

struct coorList {
    size_t X;
    size_t Y;
    uchar *data;
    struct coorList *next;
};

struct Cara {
    size_t x;
    size_t y;
    size_t height;
    size_t width;
};

struct Line {
    size_t x;
    size_t y;
    size_t height;
    size_t width;
    Cara* lcara;
};

struct Bloc {
    size_t x;
    size_t y;
    size_t height;
    size_t widthi;
    Line* lline;
}

struct Cara new_cara() {
    struct Cara cara = malloc(sizeof(struct Cara));
    cara->x = 0;
    cara->y = 0;
    cara->height = 0;
    cara->widtht = 0;
    return cara;
}

struct Line new_line() {
    struct Line line = malloc(sizeof(struct Line));
    line->x = 0;
    line->y = 0;
    line->width = 0;
    line->height = 0;
    line->lcara = NULL;
}

struct Bloc new_bloc() {
    struct Bloc bloc = malloc(sizeof(struct Bloc));
    bloc->x = 0;
    bloc->y = 0;
    bloc->width = 0;
    bloc->height = 0;
    bloc->lline = NULL;
}
#endif
