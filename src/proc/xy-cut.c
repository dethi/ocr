#include "xy-cut.h"

void HXYCut (uchar *data, size_t X, size_t Y, size_t min, size_t x, size_t y,
        struct coorList *l)
{
    //printf("[INFO] HXYCut launched %zu : %zu\n", X, Y);
    //printf("[INFO] HXYCut params %zu : %zu\n", x, y);

    if ( X < min && Y < min) {
        uchar *carac = malloc(sizeof(uchar) * X * Y);
        size_t i = 0, j = 0;
        while (i<X && j<Y) {
            carac[i + X*j] = data[(x+i) + X*(y+j)];
            if (i == X) {
                ++j;
                i = 0;
            }
            else {
                ++i;
            }
        }
        listAdd(l, carac, X, Y);
        return;
    }

    size_t *tmp = getTab(data, (char)0, X, Y, x, y, calloc(Y, sizeof(size_t)));
    size_t i = y, aux;
    while (i < y+Y) {
        //printf("[INFO] Entering while loop #%zu\n", tmp[i-y]);
        //Goes to the first line with at least 1 black pixel
        while (i < y+Y && tmp[i - y] == 255 * X)
            ++i;
        aux = i;
        //Goes to the last line with at least 1 black pixel
        while (aux < y+Y && tmp[aux - y] < 255 * X)
            ++aux;
        if (aux > i && aux < y+Y) {
            VXYCut(data, X, aux - i, min, x, i, l);
        }
        i = aux;
    }
    free(tmp);
}

void VXYCut (uchar *data, size_t X, size_t Y, size_t min, size_t x, size_t y,
        struct coorList *l)
{
    //printf("[INFO] VXYCut launched %zu : %zu\n", X, Y);
    //printf("[INFO] VXYCut params %zu : %zu\n", x, y);

    if (X < min && Y < min) {
        uchar *carac = malloc(sizeof(uchar) * X * Y);
        size_t i = 0, j = 0;
        while (i<X && j<Y) {
            carac[i + X*j] = data[(x+i) + X*(y+j)];
            if (i == X) {
                ++j;
                i = 0;
            }
            else {
                ++i;
            }
        }
        listAdd(l, carac, X, Y);
        return;
    }

    size_t *tmp = getTab(data, 1, X, Y, x, y, calloc(X, sizeof(size_t)));
    size_t i = x, aux;
    while (i < x+X) {
        //Goes to the first column with at least 1 black pixel
        while (i < x+X && tmp[i - x] == 255 * Y)
            ++i;
        aux = i;
        //Goes to the last column with at least 1 black pixel
        while (aux < x+X && tmp[aux - x] < 255 * Y)
            ++aux;
        if (aux > i && aux < x+X) {
            HXYCut(data, aux - i, Y, min, i, y, l);
        }
        i = aux;
    }
    free(tmp);
}

size_t* getTab(uchar* img, char vert, size_t X, size_t Y, size_t x, size_t y,
        size_t *tmp)
{
    uchar aux[X][Y];
    size_t i = 0, j = 0;
    while (i < X-1 || j < Y-1) {
        aux[i][j] = img[(x+i) + X*(y+j)];
        i++;
        if (i == X) {
            i = 0;
            ++j;
        }
    }
    if (vert) {
        for (i = 0; i < X; ++i) {
            for (j = 0; j < Y; ++j)
                tmp[i] += (size_t)aux[i][j];
        }
    }
    else {
        for (j = 0; j < Y; ++j) {
            for (i = 0; i < X; ++i)
                tmp[j] += (size_t)aux[i][j];
        }
    }
    return tmp;
}
