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

struct xyHelper* getPos(uchar *c, size_t size);
size_t* getTab(uchar *tab, char vert, size_t X, size_t Y, size_t x, size_t y);
void XYCut(uchar *tab, char vert, size_t X, size_t Y, size_t min, size_t x, \
        size_t y, struct coorList *helper);
void helperAdd(struct coorList *f, size_t x, size_t y, uchar *tab);

#endif
