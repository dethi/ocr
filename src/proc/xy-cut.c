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
