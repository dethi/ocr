#include "xy-cut.h"

size_t* getTab(uchar *tab, char vert, size_t X, size_t Y, size_t x, size_t y)
{
    size_t *ans;
    printf("[INFO] GetTab STARTING\n");
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

void XYCut(uchar *tab, char vert, size_t X, size_t Y, size_t min, size_t x, \
        size_t y, struct coorList *helper)
{
    vert = (vert % 2) - 1;
    printf("[INFO] XYCut launching DONE\n");
    if (X <= min && Y <= min) {
        uchar *data = malloc(sizeof(uchar) * X * Y);
        for (size_t i = 0; i+x+x*y < X * Y; ++i)
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
            XYCut(tab, vert+1, (vert ? X : (aux - i)), (vert ? (aux - i) : Y), \
                    min, (vert ? x : x+i), (vert ? y+i : y), helper);
        i = aux + 1;
    }
}

void helperAdd(struct coorList *f, size_t x, size_t y, uchar *tab)
{
    /*
     * Partie eventuellement a refaire...
    while(f->next != NULL)
        f = f->next;
    f->next = malloc(sizeof(struct coorList));
    f = f->next;
    f->X = x;
    f->Y = y;
    f->data = tab;
    f->next = NULL;
    */
    struct coorList *tmp = f->next;
    printf("[INFO] Helping...\n");
    struct coorList *aux = malloc(sizeof(struct coorList));
    f->next = aux;
    aux->data = tab;
    aux->X = x;
    aux->Y = y;
    aux->next = tmp;
    /*
     * Pour tester, ce n'est pas le probleme
    f->data[0] = 255;
    f->data[1] = 0;
    f->data[2] = 255;
    f->data[3] = 0;
    f->X = 4;
    f->Y = 1;
    f->next = NULL;
    */
}
