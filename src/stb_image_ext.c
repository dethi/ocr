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

// (x,y) axis to tab index
static inline
int xytoi(int i, int j, t_img_desc* img)
{
    return (img->comp) * (i + (img->x) * j);
}

void grey_scale(t_img_desc* img)
{
    if (img->comp != 3)
        return;

    for (int i = 0; i < img->x * img->y; i++) {
        img->data[i] = grey(img->data[(i * 3)], img->data[(i * 3) + 1],
                img->data[(i * 3) + 2]);
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

void binarize(t_img_desc* img)
{
    int th = 127;
    int m = img->x * img->y;

    for (int i = 0; i < m; i++)
        img->data[i] = (img->data[i] >= th) ? 255 : 0;
}

// currently it don't correctly work
void average_filter(t_img_desc* img)
{
    if (img->comp != 3)
        return;

    uchar *result = calloc(img->x * img->y * img->comp, sizeof(char));
    if (!result)
        exit(EXIT_FAILURE);

    for (int y = 1; y < img->y - 1; y++) {
        for (int x = 1; x < img->x - 1; x++) {
            int k = xytoi(x, y, img);

            //result[r]
            result[k] =
                (img->data[xytoi(x - 1, y - 1, img)] +
                img->data[xytoi(x, y - 1, img)] +
                img->data[xytoi(x + 1, y - 1, img)] +
                img->data[xytoi(x - 1, y, img)] +
                img->data[k] +
                img->data[xytoi(x + 1, y, img)] +
                img->data[xytoi(x - 1, y + 1, img)] +
                img->data[xytoi(x, y + 1, img)] +
                img->data[xytoi(x + 1, y + 1, img)]) / 9;

            //result[g]
            result[k + 1] =
                (img->data[xytoi(x - 1, y - 1, img) + 1] +
                img->data[xytoi(x, y - 1, img) + 1] +
                img->data[xytoi(x + 1, y - 1, img) + 1] +
                img->data[xytoi(x - 1, y, img) + 1] +
                img->data[k + 1] +
                img->data[xytoi(x + 1, y, img) + 1] +
                img->data[xytoi(x - 1, y + 1, img) + 1] +
                img->data[xytoi(x, y + 1, img) + 1] +
                img->data[xytoi(x + 1, y + 1, img) + 1]) / 9;

            //result[b]
            result[k + 2] =
                (img->data[xytoi(x - 1, y - 1, img) + 2] +
                img->data[xytoi(x, y - 1, img) + 2] +
                img->data[xytoi(x + 1, y - 1, img) + 2] +
                img->data[xytoi(x - 1, y, img) + 2] +
                img->data[k + 2] +
                img->data[xytoi(x + 1, y, img) + 2] +
                img->data[xytoi(x - 1, y + 1, img) + 2] +
                img->data[xytoi(x, y + 1, img) + 2] +
                img->data[xytoi(x + 1, y + 1, img) + 2]) / 9;
        }
    }

    free(img->data);
    img->data = result;
}
