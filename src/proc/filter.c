#include "filter.h"

// Div: 9
const char mask_median[] =
{
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
};

// Div: 273
const char mask_gaussien[] =
{
	1,  4,  7,  4, 1,
	4, 16, 26, 16, 4,
	7, 26, 41, 26, 7,
	4, 16, 26, 16, 4,
	1,  4,  7,  4, 1
};

// Div: 9
const char mask_sharpening[] =
{
    -1, -1, -1,
    -1, 17, -1,
    -1, -1, -1
};

static inline
int min(int a, int b)
{
    return (a < b) ? a : b;
}

static inline
int max(int a, int b)
{
    return (a > b) ? a : b;
}

static
int coor(int x, int y, int i, int j, size_t n, t_img_desc *img)
{
    int cx = (x + n/2) - i;
    int cy = (y + n/2) - j;

    if (0 > cx || cx >= img->x) {
        if (cx < 0)
            cx -= 2 * cx - 1;
        else
            cx -= 2 * (cx - (img->x - 1)) + 1;
    }
    if (0 > cy || cy >= img->y) {
        if (cy < 0)
            cy -= 2 * cy - 1;
        else
            cy -= 2 * (cy - (img->y - 1)) + 1;
    }

    return xytoi(cx, cy, img);
}

void filter_mask(t_img_desc *img, const char *mask, int sum_mask, int n)
{
    uchar *tmp = malloc(sizeof(char) * img->x * img->y * img->comp);
    if (!tmp)
        exit(EXIT_FAILURE);

    for (int k = 0; k < img->comp; ++k) {
        for(int y = 0; y < img->y; ++y) {
            for(int x = 0; x < img->x; ++x) {
                int sum = 0;

                for(int j = 0; j < n; ++j) {
                    for(int i = 0; i < n; ++i) {
                        sum += img->data[coor(x, y, i, j, n, img) + k] *
                            mask[i + n * j];
                    }
                }

                tmp[xytoi(x, y, img) + k] = max(0, min(255, sum / sum_mask));
            }
        }
    }

    free(img->data);
    img->data = tmp;
}
