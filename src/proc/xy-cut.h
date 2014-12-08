#ifndef _XY_CUT_H_
#define _XY_CUT_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../stb_image/stb_image_ext.h"
#include "list.h"

struct xyHelper {
    size_t length;
    size_t begin;
};

void HXYCut(uchar *data, size_t X, size_t Y, size_t min, size_t x, size_t y,
        struct coorList *l, size_t GX);
void VXYCut(uchar *data, size_t X, size_t Y, size_t min, size_t x, size_t y,
        struct coorList *l, size_t GX);
size_t* getTab(uchar *img, char vert, size_t X, size_t Y, size_t x, size_t y,
        size_t *tmp);

#endif
