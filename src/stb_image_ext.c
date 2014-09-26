#include <stdlib.h>
#include <stdio.h>
#include "stb_image.h"
#include "stb_image_ext.h"

const t_img_desc T_IMG_DESC_DEFAULT = {
    .data = NULL,
    .x = 0,
    .y = 0,
    .comp = 0
};

t_img_desc load_image(char* filename, int comp)
{
    t_img_desc img = T_IMG_DESC_DEFAULT;
    img.data = stbi_load(filename, &img.x, &img.y, &img.comp, comp);

    if (img.x == 0 || img.y == 0) {
        perror(stbi_failure_reason());
        exit(EXIT_FAILURE);
    }

    return img;
}

void free_image(t_img_desc* img)
{
    free(img->data);
    *img = T_IMG_DESC_DEFAULT;
}

void grey_scale(t_img_desc* tab)
{
    if (tab.comp == 3) {
        int i = 0, j;
        int c = 0;

        unsigned char* new_data = malloc(sizeof(char) * x  * y);
        if (new_data)
            exit(EXIT_FAILURE);

        while (j<tab->y) {
            j = 0;
            while(i<tab->x) {
                c = coor(i,j);
                // save grey value in the new shorter array
                new_data[i+ (tab->x)*j] = grey(tab->data[c],tab->data[c+1],tab->data[c+2]);
                i++;
            }
            j++;
        }

        free(tab->data);
        tab->data = new_data;
        tab->comp = 1;
    }
}

int coor(int i, int j, t_img_desc* img)
{
    //img->comp = number of pixels composants
    return (img->comp)*(i+(img->x)*j);
}

char grey(char r, char g, char b)
{
    return (char)(0.299*r + 0.587*g + 0.144*b);
}
