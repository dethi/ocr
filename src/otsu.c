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
    uint *h;
    h = histogram_fast(img);
    return blacAndWhite(thresold(h), img);
}

int thresold(uint* h)
{
    uint *ans = malloc(sizeof(int) * 256);
    int v1 = 0, v2 = 0, p1 = 0, p2 = 0;
    //Loop that calculates the best thresold
    for (int i = 0; i < 256; i++) {
        //For the element under the current thresold
        for (int j = 0; j < i; j++) {
            v1 += (j-i)*(j-i);
            p1 += h[j];
        }
        v1 = (1/i)*v1;
        //For the element up the current thresold
        for (int k = i; k < 256; k++) {
            v2 += (k-i)*(k-i);
            p2 += h[k];
        }
        v2 = (1/i)*v2;
        ans[i] = p1*v1+p2*v2;
    }
    int min = ans[0];
    for (int l = 1; l < 256; l++) {
        if (ans[l]<min)
            min = ans[i];
    }
    return min;
}

t_img_desc* blackAndWhite(int t, t_img_desc* img)
{
    for (int i = 0; i < (img->x)*(img->y); i++)
    {
        if (img->data[i] >= t)
            img->data[i] = 255;
        else
            img->data = 0;
    }
    return img;
}
