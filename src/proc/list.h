#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char uchar;

struct coorList {
    size_t X;
    size_t Y;
    uchar *data;
    struct coorList *next;
};

void listAdd(struct coorList *l, uchar *data, size_t X, size_t Y);

#endif
