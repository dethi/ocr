#include "xy-cut.h"

struct xyHelper* getPos(uchar *c, size_t size)
{
    size_t i = 0, aux = 0;
    struct xyHelper *ans = malloc(sizeof(struct xyHelper));
    ans->length = 0;
    ans->begin = 0;
    while (i < size) {
        if (c[i] == 0)
            ++i;
        else {
            aux = i + 1;
            while (aux < size && c[aux] == 255)
                ++aux;
            if (aux - i > ans->length) {
                ans->length = aux - i;
                ans->begin = i;
            }
            i = aux;
        }
    }
    return ans;
}

size_t* getTab(uchar *tab, char vert, size_t X, size_t Y, size_t x, size_t y)
{
    size_t *ans;
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
        ans = calloc(X, sizeof(uchar));
        while (x < X) {
            y = 0;
            while (y < Y) {
                ans[x] += tab[x + x*y];
                ++y;
            }
            ++x;
        }
    }
    return ans;
}
void XYCut(uchar *tab, char vert, size_t X, size_t Y, size_t min, size_t x, size_t y, struct coorList *helper)
{
    if (X <= min && Y <= min) {
        uchar *data = malloc(sizeof(uchar) * X * Y);
        for (size_t i = 0; i < X * Y; ++i)
            data[i] = tab[i + x + x * y];
        helperAdd(helper, x, y, data);
        return;
    }
    size_t *tmp = getTab(tab, vert, X, Y, x, y);
    size_t foo = (vert ? Y : X), i = (vert ? y : x), aux = 0;
    while (i < foo) { 
        while (i < foo && tmp[i] == 255 * foo)
            ++i;
        aux = i;
        while (aux < foo && tmp[aux] != 255 * foo)
            ++aux;
        if (aux != i)
            XYCut(tab, !vert, (vert ? X : (aux - i)), (vert ? (aux - i) : Y), min, (vert ? x : x+i), (vert ? y+i : y), helper);
        i = aux + 1;
    }
}

void helperAdd(struct coorList *f, size_t x, size_t y, uchar *tab)
{
    while(f->next != NULL)
        f = f->next;
    f->next = malloc(sizeof(struct coorList));
    f->X = x;
    f->Y = y;
    f->data = tab;
    f->next = NULL;
}
