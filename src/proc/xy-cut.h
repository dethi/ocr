#ifndef _XY_CUT_H_
#define _XY_CUT_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../stb_image/stb_image_ext.h"
#include "detection.h"

struct xyHelper {
    size_t length;
    size_t begin;
};

size_t* getTab(uchar *tab, char vert, size_t X, size_t Y, size_t x, size_t y, size_t *tmp);
void VXYCut(uchar *tab, size_t X, size_t Y, size_t min, size_t x, size_t y, \
        struct coorList *helper);
void HXYCut(uchar *tab, size_t X, size_t Y, size_t min, size_t x, size_t y, \
        struct coorList *helper);
void listAdd(struct coorList *f, uchar *data, size_t x, size_t y);
void reverse(struct coorList *f);

#endif
