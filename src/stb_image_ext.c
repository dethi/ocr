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

t_img_desc* load_image(char* filename, int comp)
{
    t_img_desc *img = malloc(sizeof(t_img_desc));
    if (!img)
        exit(EXIT_FAILURE);

    *img = T_IMG_DESC_DEFAULT;
    img->data = stbi_load(filename, &(img->x), &(img->y), &(img->comp), comp);

    if (img->x == 0 || img->y == 0) {
        perror(stbi_failure_reason());
        exit(EXIT_FAILURE);
    }

    return img;
}

void free_image(t_img_desc* img)
{
    free(img->data);
    free(img);
    img = NULL;
}

void grey_scale(t_img_desc* img)
{
    if (img->comp != 3)
        return;

    int i = 0, c = 0, j = 0;

    while (j < img->y) {
        i = 0;
    
        while(i < img->x) {
            c = coor(i,j, img);
            // save grey value in the array
            img->data[i + (img->x) * j] = grey(
                    img->data[c], img->data[c + 1], img->data[c + 2]);
            i++;
        }
        
        j++;
    }
    
    // make the array shorter
    unsigned char *tmp = realloc(img->data, sizeof(char) * img->x * img->y);
    if (!tmp) {
        free_image(img);
        exit(EXIT_FAILURE);
    }

    img->data = tmp;
    img->comp = 1;
}

int coor(int i, int j, t_img_desc* img)
{
    return (img->comp)*(i+(img->x)*j);
}

char grey(char r, char g, char b)
{
    return (char)(0.299 * r + 0.587 * g + 0.144 * b);
}

