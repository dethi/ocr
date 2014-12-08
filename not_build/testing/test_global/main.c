#include "main.h"

int main(int argc, char *argv[])
{
    assert(argc > 1);

    t_img_desc *img = load_image(argv[1], 3);
    printf("[INFO] Load %s (%ix%i -- %i)\n", argv[1], img->x, img->y, img->comp);

    grey_scale(img);
    binarize_otsu(img);

    printf("[INFO] Rotation of %.2f degree\n", rotate_img(img));
    filter_median(img);

    write_image("out_img.png", img);
    printf("[INFO] Write img_out.png\n");
    free_image(img);

    return EXIT_SUCCESS;
}
