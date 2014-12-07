#include "xy-cut.h"

size_t* getTab(uchar *tab, char vert, size_t X, size_t Y, size_t x, size_t y)
{
    size_t *ans;
    printf("[INFO] GetTab call %d : %d\n", (int)x, (int)y);
    if (vert) {
        ans = calloc(Y, sizeof(size_t));
        while (y < Y) {
            x = 0;
            while (x < X) {
                ans[y] += tab[x + x*y];
                ++x;
            }
            ++y;
        }
    }
    else {
        ans = calloc(X, sizeof(size_t));
        while (x < X) {
            y = 0;
            while (y < Y) {
                ans[x] += tab[x + x*y];
                ++y;
            }
            ++x;
        }
    }
    printf("[INFO][SOME VALUES] %d : %d : %d\n", (int)ans[0], (int)ans[100], (int)ans[355]);
    return ans;
}

void HXYCut(uchar *tab, size_t X, size_t Y, size_t min, size_t x, size_t y, \
        struct coorList *helper)
{
    printf("[INFO] Starting HXYCut\n");
    if (X <= min && Y <= min) {
        uchar *data = malloc(sizeof(uchar) * X * Y);
        for (size_t i = 0; i+x+x*y < X * Y; ++i)
            data[i] = tab[i + x + x * y];
        helperAdd(helper, x, y, data);
        return;
    }
    size_t *tmp = getTab(tab, (char)0, X, Y, x, y);
    size_t foo = Y, i = x, aux = 0;
    while (i < foo) {
        while (i < foo && tmp[i] == 255 * foo)
        {++i;}
        aux = i;
        while (aux < foo && tmp[aux] != 255 * foo)
            ++aux;
        if (aux != i) {
            VXYCut(tab, (aux - i), Y, min, x+i, y, helper);
        }
        i = aux + 1;
    }
    free(tmp);
}

void VXYCut(uchar *tab, size_t X, size_t Y, size_t min, size_t x, size_t y, \
        struct coorList *helper)
{
    printf("[INFO] Starting VXYCut\n");
    if (X <= min && Y <= min) {
        uchar *data = malloc(sizeof(uchar) * X * Y);
        for (size_t i = 0; i+x+x*y < X * Y; ++i)
            data[i] = tab[i + x + x * y];
        helperAdd(helper, x, y, data);
        return;
    }
    size_t *tmp = getTab(tab, (char)1, X, Y, x, y);
    size_t foo = X, i = y, aux = 0;
    while (i < foo) {
        while (i < foo && tmp[i] == 255 * foo)
        {++i;}
        aux = i;
        while (aux < foo && tmp[aux] != 255 * foo)
            ++aux;
        if (aux != i) {
            HXYCut(tab, X, (aux - i), min, x, y+i, helper);
        }
        i = aux + 1;
    }
    free(tmp);
}

void helperAdd(struct coorList *f, size_t x, size_t y, uchar *tab)
{
    struct coorList *tmp = f->next;
    printf("[INFO] Helping...\n");
    struct coorList *aux = malloc(sizeof(struct coorList));
    f->next = aux;
    aux->data = tab;
    aux->X = x;
    aux->Y = y;
    aux->next = tmp;
}
