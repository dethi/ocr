#include <stdlib.h>
#include <stdio.h>
#include "stb_image.h"
#include "stb_image_ext.h"

const t_img_desc T_IMG_DESC_DEFAULT = {
    .data = NULL,
    .x = 0,
    .y = 0,
    .comp = 0
};

t_img_desc load_image(char* filename, int comp)
{
    t_img_desc img = T_IMG_DESC_DEFAULT;
    img.data = stbi_load(filename, &img.x, &img.y, &img.comp, comp);

    if (img.x == 0 || img.y == 0) {
        perror(stbi_failure_reason());
        exit(EXIT_FAILURE);
    }

    return img;
}

void free_image(t_img_desc* img)
{
    free(img->data);
    *img = T_IMG_DESC_DEFAULT;
}
