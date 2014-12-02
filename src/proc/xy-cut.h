#ifndef _XY_CUT_H_
#define _XY_CUT_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../stb_image/stb_image_ext.h"

struct xyHelper {
    size_t length;
    size_t begin;
};

struct xyHelper* getPos(uchar *c, size_t size);

uchar* getVerticalTab(t_img_desc *img);

#endif
