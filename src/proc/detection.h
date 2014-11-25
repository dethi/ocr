#ifndef _DETECTION_H_
#define _DETECTION_H_

#include <stdlib.h>
#include <stdio.h>
#include "../stb_image/stb_image_ext.h"

struct lineData {
    size_t X;
    size_t Y;
    size_t length;
    size_t height;
}

void RLSA(t_img_desc *img, int i, int j);
size_t charX(t_img_desc *img, struct lineData *ld);

#endif
