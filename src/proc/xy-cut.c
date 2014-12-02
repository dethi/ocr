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

uchar* getVerticalTab(t_img_desc *img)
{
    uchar *ans = malloc(sizeof(uchar)*img->x*img->y);
    int x = 0, y = 0, i = 0;
    while(x * y < img->x*img->y) {
        ans[i] = img->data[x + x*y];
        ++y;
        if (y == img->y) {
            ++x;
            y = 0;
        }
        ++i;
    }
    return ans;
}
