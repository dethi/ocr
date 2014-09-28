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

t_img_desc* load_image(char* filename, int comp)
{
    t_img_desc *img = malloc(sizeof(t_img_desc));
    if (!img)
        exit(EXIT_FAILURE);

    *img = T_IMG_DESC_DEFAULT;
    img->data = stbi_load(filename, &(img->x), &(img->y), &(img->comp), comp);

    if (img->x == 0 || img->y == 0) {
        perror(stbi_failure_reason());
        exit(EXIT_FAILURE);
    }

    return img;
}

void free_image(t_img_desc* img)
{
    free(img->data);
    free(img);
    img = NULL;
}

int coor(int i, int j, t_img_desc* img)
{
    return (img->comp)*(i+(img->x)*j);
}

void grey_scale(t_img_desc* img)
{
    if (img->comp != 3)
        return;

    int i = 0, c = 0, j = 0;

    while (j < img->y) {
        i = 0;

        while(i < img->x) {
            c = coor(i,j, img);
            // save grey value in the array
            img->data[i + (img->x) * j] = grey(
                    img->data[c], img->data[c + 1], img->data[c + 2]);
            i++;
        }

        j++;
    }

    // make the array shorter
    uchar *tmp = realloc(img->data, sizeof(char) * img->x * img->y);
    if (!tmp) {
        free_image(img);
        exit(EXIT_FAILURE);
    }

    img->data = tmp;
    img->comp = 1;
}

uchar grey(uchar r, uchar g, uchar b)
{
    return (uchar)(0.21 * r + 0.72 * g + 0.07 * b);
}

uint* histogram(t_img_desc* img)
{
    uint *h = calloc(256, sizeof(int));
    if (!h) {
        free_image(img);
        exit(EXIT_FAILURE);
    }

    uchar *ptr = img->data;
    const uchar *end = &img->data[(img->x * img->y * img->comp) - 1];

    while(ptr < end)
        h[*ptr++]++;

    return h;
}

// Faster than histogram()
// Explanation: Counting bytes fast - http://goo.gl/5LZ7fE
uint* histogram_fast(t_img_desc* img)
{
    uint *h = malloc(sizeof(int) * 256);
    if (!h) {
        free_image(img);
        exit(EXIT_FAILURE);
    }

    uchar *ptr = img->data;
    const uchar *end = &img->data[(img->x * img->y * img->comp) - 1];

    uint count1[256] = { 0 };
    uint count2[256] = { 0 };
    uint count3[256] = { 0 };
    uint count4[256] = { 0 };

    while (ptr < end - 3) {
        count1[*ptr++]++;
        count2[*ptr++]++;
        count3[*ptr++]++;
        count4[*ptr++]++;
    }

    while (ptr < end)
        count1[*ptr++]++;

    for (int i = 0; i < 256; i++)
        h[i] = count1[i] + count2[i] + count3[i] + count4[i];

    return h;
}

