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
    assert(img);

    *img = T_IMG_DESC_DEFAULT;
    img->data = stbi_load(filename, &(img->x), &(img->y), &(img->comp), comp);

    if (img->x == 0 || img->y == 0) {
        perror(stbi_failure_reason());
        exit(EXIT_FAILURE);
    }

    return img;
}

int write_image(char* filename, t_img_desc* img)
{
    return stbi_write_png(filename, img->x, img->y, img->comp, img->data,
            img->x * img->comp);
}

void free_image(t_img_desc* img)
{
    free(img->data);
    free(img);
    img = NULL;
}

int grey2RGB(t_img_desc* img)
{
    if (img->comp != 1)
        return EXIT_FAILURE;

    uchar *ptr = realloc(img->data, sizeof(char) * img->x * img->y * 3);
    assert(ptr);

    for (size_t i = (img->x * img->y) - 1; i != 0; --i) {
        ptr[3 * i] = ptr[i];
        ptr[3 * i + 1] = ptr[i];
        ptr[3 * i + 2] = ptr[i];
    }

    img->data = ptr;
    img->comp = 3;
    return EXIT_SUCCESS;
}

uint* histogram(t_img_desc* img)
{
    uint *h = calloc(256, sizeof(int));
    assert(h);

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
    assert(h);

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
