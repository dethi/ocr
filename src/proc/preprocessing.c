#include "preprocessing.h"

const char mask_median[] =
{
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
};

const char mask_gaussien[] =
{
	1, 4, 7, 4, 1,
	4, 16, 26, 16, 4,
	7, 26, 41, 26, 7,
	4, 16, 26, 16, 4,
	1, 4, 7, 4, 1
};

static inline
double pw2(double x)
{
    return x * x;
}

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

static inline
uchar grey(uchar r, uchar g, uchar b)
{
    return (r + g + b) / 3;
}

/*
static inline
uchar human_grey(uchar r, uchar g, uchar b)
{
    return (uchar)(0.21 * r + 0.72 * g + 0.07 * b);
}
*/

static
void binarize(uchar* data, int size, int th)
{
    for (int i = 0; i < size; i++)
        data[i] = (data[i] > th) ? 255 : 0;
}

static
int thresold(uint *h, size_t size)
{
    double prob[256];
    size_t min_i = 256;
    double min_var = DBL_MAX;

    for (size_t i = 0; i < 256; ++i)
        prob[i] = (double)h[i] / (double)size;

    for (size_t j = 1; j < 255; ++j) {
        double m_under = 0, c = 0;
        for (size_t k = 0; k < j; ++k) {
            m_under += k * prob[k];
            c += prob[k];
        }

        // next iteration if a class is empty
        if (c == 0 || c == 1)
            continue;
        m_under /= c;

        double m_up = 0;
        for (size_t k = j; k < 256; ++k)
            m_up += k * prob[k];
        m_up /= (1 - c);

        double var = 0;
        for (size_t k = 0; k < j; ++k)
            var += pw2(k - m_under) * prob[k];
        for (size_t k = j; k < 256; ++k)
            var += pw2(k - m_up) * prob[k];

        if (min_var > var) {
            min_i = j;
            min_var = var;
        }
    }

    return min_i;
}

//Function that returns the mirror pixel when (x,y)
//is out of the img
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

void binarize_basic(t_img_desc* img)
{
    if (img->comp != 1)
        return;

    binarize(img->data, img->x * img->y, 127);
}

void binarize_otsu(t_img_desc* img)
{
    if (img->comp != 1)
        return;

    uint *h = histogram_fast(img);
    int th = thresold(h, img->x * img->y);
    free(h);

    binarize(img->data, img->x * img->y, th);
}

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

void filter_mask(t_img_desc *img, const char *mask, int sum_mask, int n)
{
    uchar *tmp = calloc(img->x * img->y * img->comp, sizeof(char));
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
