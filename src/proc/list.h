#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include "../stb_image/stb_image_ext.h"

struct coorList {
    size_t X;
    size_t Y;
    uchar *data;
    struct coorList *next;
};

struct coorList* listInit();
void listAdd(struct coorList *l, uchar *data, size_t X, size_t Y);
void listReverse(struct coorList *l);
int listLen(struct coorList *l);
void listFree(struct coorList *l);

#endif
