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

uchar* getVerticalTab(uchar *tab, size_t length, size_t Y)
{
    uchar *ans = malloc(sizeof(uchar)*length);
    size_t x = 0, y = 0, i = 0;
    while(x * y < length) {
        ans[i] = tab[x + x*y];
        ++y;
        if (y == Y) {
            ++x;
            y = 0;
        }
        ++i;
    }
    return ans;
}

void XYCut(uchar *tab, char vert, size_t length, size_t X, size_t Y)
{
    ++X; //Sinon il rale
    //Si on doit le traiter en vertical
    struct xyHelper *help;
    if(vert)
        help = getPos(getVerticalTab(tab, length, Y), length);
    help = getPos(tab, length);
    help->length += 1; //Sinon il rale
    //Faire appel vers les deux parties
}
