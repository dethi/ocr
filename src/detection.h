#ifndef _DETECTION_H_
#define _DETECTION_H

#include <stdlib.h>
#include <stdio.h>
#include "stb_image/stb_image_ext.h"

void RLSA(t_img_desc *img, int i, int j);
void trans_RLSA(uchar *tab, int size, int c);

#endif
