#ifndef _DETECTION_H_
#define _DETECTION_H_

#include <stdlib.h>
#include <stdio.h>
#include "../stb_image/stb_image_ext.h"

struct coorList {
    size_t X;
    size_t Y;
    size_t length;
    size_t height;
    struct coorList *next;
};

//Apply RLSA with i and j parameters to img picture
//FIX ME (Full of buggs)
void RLSA(t_img_desc *img, int i, int j);

//Returns the X coor of the first character in a line
size_t charX(t_img_desc *img, struct coorList *ld);

//Returns the length of the character located at begin in line ld
size_t charLength(t_img_desc *img, struct coorList *ld, size_t begin);

//Returns a coorList of all lines in RLSAed img
//FIX ME (Detect titles as normal lines)
struct coorList* getLines(t_img_desc *img);

//Used for getLines
//void dimLine(struct coorList *l, t_img_desc *img, size_t i, size_t j);
#endif
