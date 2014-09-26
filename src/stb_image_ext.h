#ifndef HEADER_STB_IMAGE_EXT_H
#define HEADER_STB_IMAGE_EXT_H

typedef struct {
    unsigned char* data;
    int x;
    int y;
    int comp;
} t_img_desc;

t_img_desc load_image(char* filename, int comp);
void free_image(t_img_desc* img);
void grey_scale(t_img_desc* tab);
int coor(int i, int j, t_img_desc* img);
char grey(char r, char g, char b);

#endif
