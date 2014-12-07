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

#endif
