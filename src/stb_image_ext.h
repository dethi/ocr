#ifndef _STB_IMAGE_EXT_H_
#define _STB_IMAGE_EXT_H_

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct {
    uchar* data;
    int x;
    int y;
    int comp;
} t_img_desc;

t_img_desc* load_image(char* filename, int comp);
int write_image(char* filename, t_img_desc* img);
void free_image(t_img_desc* img);
void grey_scale(t_img_desc* tab);
uchar human_grey(uchar r, uchar g, uchar b);
uint* histogram(t_img_desc* img);
uint* histogram_fast(t_img_desc* img);
void binarize(uchar* data, int size, int th);
void binarize_basic(t_img_desc* img);
void binarize_otsu(t_img_desc* img);
int thresold(uint* h);
void average_filter(t_img_desc* img);
void gaussian_blur(t_img_desc* img, float sigma);

#endif
