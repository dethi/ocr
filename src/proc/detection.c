#include "detection.h"

vect y_line;
vect hist_y;
vect hist_x;
coor *charact;
int charact_length = 0;
int number_of_lines = 0;

static inline int get_x(t_img_desc * img, int index)
{
    return (index % (img->x));
}

static
void init_hist(t_img_desc * img)
{
    hist_x.length = img->x;
    hist_y.length = img->y;
    hist_x.element = calloc(hist_x.length, sizeof(int));
    assert(hist_x.element);
    hist_y.element = calloc(hist_y.length, sizeof(int));
    assert(hist_y.element);

    y_line.length = img->y;
    y_line.element = calloc(y_line.length, sizeof(int));
    assert(y_line.element);
}

static
void init_coordinate(t_img_desc * img)
{
    charact = malloc(sizeof(coor) * (y_line.length / 2) * img->x);
    assert(charact);
}

static
void make_hist_y(t_img_desc * img)
{
    for (int i = 0; i < img->x * img->y; i++) {
        if (img->data[i] == 0) {
            hist_y.element[i / img->x] += 1;
        }
    }
}

static
void get_y_line()
{
    int i = 0, j = 0;

    while (i < hist_y.length) {
        if (hist_y.element[i] > 0) {
            y_line.element[j] = i;
            j++;
            number_of_lines++;
            while (hist_y.element[i] > 0) {
                i++;
            }
            y_line.element[j] = i - 1;
            j++;
        }
        i++;
    }
    y_line.length = j;
}

static
void set_line(t_img_desc * img)
{
    for (int i = 0; i < img->x * img->y; i++) {
        for (int j = 0; j < (y_line.length); j++) {
            if (y_line.element[j] == i / img->x)
                img->data[i] = 200;
        }
    }
}

static
void make_hist_x(t_img_desc * img)
{
    int x = img->x;
    hist_x.length = y_line.length * img->x;
    hist_x.element = calloc(hist_x.length, sizeof(int));
    assert(hist_x.element);

    for (int i = 0; i < img->x * img->y; i++) {
        for (int j = 0; j < (y_line.length); j += 2) {
            if (i / img->x >= y_line.element[j] &&
                    i / img->x <= y_line.element[j + 1]) {
                if (img->data[i] == 0)
                    hist_x.element[(j * x) + i - ((i / x) * x)] += 1;
            }
        }
    }
}

static
void get_coordinate(t_img_desc * img)
{
    int i = 0, j = 0, x = img->x, y_min, y_max;

    while (i < hist_x.length) {
        if (hist_x.element[i] > 0) {
            y_min = i / x;
            y_max = y_min + 1;
            charact[j].x_min = get_x(img, i);
            charact[j].y_min = y_line.element[y_min];
            charact[j].y_max = y_line.element[y_max];
            while (hist_x.element[i] > 0) {
                i++;
            }
            charact[j].x_max = get_x(img, i - 1);
            j++;
        }
        i++;
    }
    charact_length = j;
}

static
void set_char(t_img_desc * img)
{
    int x_min, y_min, x_max, y_max, start, end;
    for (int i = 0; i < charact_length; i++) {
        x_min = charact[i].x_min;
        x_max = charact[i].x_max;
        y_min = charact[i].y_min;
        y_max = charact[i].y_max;

        for (; y_min < y_max; y_min++) {
            start = xytoi(x_min, y_min, img);
            end = xytoi(x_max, y_min, img);
            img->data[start] = 200;
            img->data[end] = 200;
        }
    }
}

coor *detect(t_img_desc * img)
{
    init_hist(img);
    make_hist_y(img);
    get_y_line();
    set_line(img);
    make_hist_x(img);
    init_coordinate(img);
    get_coordinate(img);
    set_char(img);
    printf("There is %d lines and %d characts detected\n",
            number_of_lines, charact_length);

    return charact;
}

t_img_desc *get_data(t_img_desc * img, coor c)
{
    assert(img->comp == 1);

    int w = c.x_max - c.x_min;
    int h = c.y_max - c.y_min;

    t_img_desc *new = malloc(sizeof(t_img_desc));
    assert(new);

    new->x = w;
    new->y = h;
    new->comp = 1;

    new->data = malloc(sizeof(char) * new->x * new->y);
    assert(new->data);

    for (int i = 0; i < new->x; ++i) {
        for (int j = 0; j < new->y; ++j) {
            new->data[xytoi(i, j, new)] =
                img->data[xytoi(c.x_min + i, c.y_min + j, img)];
        }
    }

    return new;
}
