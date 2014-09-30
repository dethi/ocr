//Functions that apply Otsu's method to binarize the image
#include<stdio.h>
#include<stdlib.h>
#include"stb_image.h"
#include"stb_image_ext.h"
#include"otsu.h"

void binarize(t_image_desc* img)
{
    t_img_desc *ans = malloc(sizeof(t_img_desc));
    if (!ans)
        exit(EXIT_FAILURE);
    ans = otsu(img);
    //realloc?
    free(img);
    img = ans;
}

t_img_desc* otsu(t_img_desc* img)
{
    //Fix me
    uint *h = malloc(sizeof(int) * 256);
    h = histogram_fast(img);
    return img;
}
