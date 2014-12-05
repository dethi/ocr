#include "main.h"

int main(int argc, char *argv[])
{
    struct gengetopt_args_info ai;
    if (cmdline_parser(argc, argv, &ai) != 0)
        exit(EXIT_FAILURE);

    /* XOR */
    if (ai.xor_given) {
        net_main();
        return EXIT_SUCCESS;
    }

    /* Check files path */
    if (ai.inputs_num == 0) {
        cmdline_parser_print_help();
        exit(EXIT_FAILURE);
    }

    /* Flag */
    int flag;
    if (ai.rotation_given)
        flag = 4;
    else if (ai.segmentation_given)
        flag = 3;
    else if (ai.binarize_given)
        flag = 2;
    else if (ai.grey_given)
        flag = 1;
    else
        flag = 0;
    /* End flag */

    /* Thresold */
    int th_flag = (ai.thresold_given &&
            strcmp("fixed", ai.thresold_arg) == 0) ? 0 : 1;

    /* Segmentation */
    int RLSA_x = 3;
    int RLSA_y = 3;

    if (ai.segmentation_given) {
        RLSA_x = ai.segmentation_arg[0];
        RLSA_y = ai.segmentation_arg[1];
    }
    /* End segmenation */

    /* Filter */
    int filter_flag = ai.filter_given;
    int filter_median_flag = 0;
    const char *mask = NULL;
    int div = 0;
    int size = 0;

    if (filter_flag) {
        if (strcmp("average", ai.filter_arg) == 0) {
            extern const char mask_avg[];
            mask = mask_avg;
            div = 9;
            size = 3;
        } else if (strcmp("gaussien", ai.filter_arg) == 0) {
            extern const char mask_gaussien[];
            mask = mask_gaussien;
            div = 273;
            size = 5;
        } else if (strcmp("sharpening", ai.filter_arg) == 0) {
            extern const char mask_sharpening[];
            mask = mask_sharpening;
            div = 9;
            size = 3;
        } else if (strcmp("median", ai.filter_arg) == 0) {
            filter_median_flag = 1;
        }
    }
    /* End filter */

    char out[1000];
    //printf("flag: %i\n", flag);

    for (unsigned i = 0; i < ai.inputs_num; ++i)
    {
        t_img_desc *img = load_image(ai.inputs[i], 3);
        printf("%s (%ix%i -- %i)", ai.inputs[i], img->x, img->y, img->comp);

        if (filter_flag) {
            if (filter_median_flag)
                filter_median(img);
            else
                filter_mask(img, mask, div, size);
        }

        if (flag > 0)
            grey_scale(img);
        if (flag > 1) {
            if (th_flag)
                binarize_otsu(img);
            else
                binarize_basic(img);
        }
        if (flag > 2)
            RLSA(img, RLSA_x, RLSA_y);
        if (flag > 3)
            printf("\trot: %f", 360 * hough(img) / 6.283);

        if (flag > 0 || filter_flag) {
            sprintf(out, "out_img%i.png", i+1);
            printf("\t\t--> %s", out);
            if (!write_image(out, img))
                writing_error();
        }

        printf("\n");
        free_image(img);
    }

    return EXIT_SUCCESS;
}

void writing_error()
{
    printf("ERROR: failed to write image\n");
    exit(EXIT_FAILURE);
}
