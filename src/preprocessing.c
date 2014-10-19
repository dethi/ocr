#include "preprocessing.h"

static inline
double pw2(double x)
{
    return x * x;
}

static inline
uchar grey(uchar r, uchar g, uchar b)
{
    return (r + g + b) / 3;
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

uchar human_grey(uchar r, uchar g, uchar b)
{
    return (uchar)(0.21 * r + 0.72 * g + 0.07 * b);
}

void binarize(uchar* data, int size, int th)
{
    for (int i = 0; i < size; i++)
        data[i] = (data[i] > th) ? 255 : 0;
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

void gaussian_blur(t_img_desc *img, uchar *mask, int sum_mask, int n)
{
    uchar *tmp = calloc(img->x * img->y, sizeof(char));

    for(int j = 0; j< img->y; ++j) {
        for(int i = 0; i< img->x; ++i) {
            for(int y = -n/2 + 1; y< n/2; ++y) {
                for(int x = -n/2 + 1; x < n/2; ++x) {
                    tmp[xytoi(i,j,img)] +=
                        img->data[coor(i - x, j - y, i, j, img)] * mask[x + n * y];
                }
            }
            tmp[xytoi(i, j, img)] /= sum_mask;
        }
    }

    free(img->data);
    img->data = tmp;
}

//Function that discriminates text and blocks of img
void RLSA(t_img_desc *img, int i, int j)
{
    uchar *tab = malloc(sizeof(char) * img->x);
    uchar *tmp = malloc(sizeof(char) * img->x * img ->y);

    for (int row = 0; row < img->y; ++row) {
        for (int x = 0; x < img->x; ++x)
            tab[x] = img->data[row * img->x + x];

        trans_RLSA(tab, img->x, i);

        for (int x = 0; x < img->x; ++x)
            tmp[row * img->x + x] = tab[x];
    }

    tab = realloc(tab, sizeof(char) * img->y);

    for (int col = 0; col < img->x; ++col) {
        for (int y = 0; y < img->y; ++y)
            tab[y] = img->data[col + img->x * y];

        trans_RLSA(tab, img->y, j);

        for (int y = 0; y < img->y; ++y)
            tmp[col + img->x * y] = tab[y];
    }

    free(tab);
    free(img->data);
    img->data = tmp;
}

void trans_RLSA(uchar *tab, int size, int c)
{
    int count;
    for (int i = 0; i < size; ++i) {
        if (tab[i] == 0) {
            count = 1;

            while(tab[i+count] == 0 && i+count < size)
                ++count;

            if (count > c) {
                while (count >= 0) {
                    tab[i+count] = 0;
                    --count;
                }
            }
            else {
                while (count >= 0) {
                    tab[i+count] = 1;
                    --count;
                }
            }
        }
    }
}
