#include "main.h"

int main(int argc, char *argv[])
{
    struct gengetopt_args_info ai;
    if (cmdline_parser(argc, argv, &ai) != 0)
        exit(EXIT_FAILURE);

    if (ai.inputs_num == 0) {
        cmdline_parser_print_help();
        exit(EXIT_FAILURE);
    }

    int flag;
    if (ai.binarize_given)
        flag = 2;
    else if (ai.grey_given)
        flag = 1;
    else
        flag = 0;

    int th_flag = (ai.thresold_given &&
            strcmp("fixed", ai.thresold_arg) == 0) ? 0 : 1;

    char out[1000];
    printf("flag: %i\n", flag);

    for (unsigned i = 0; i < ai.inputs_num; ++i)
    {
        t_img_desc *img = load_image(ai.inputs[i], 3);

        printf("%s (%ix%i)", ai.inputs[i], img->x, img->y);

        if (flag > 0)
            grey_scale(img);
        if (flag > 1) {
            if (th_flag)
                binarize_otsu(img);
            else
                binarize_basic(img);
        }

        if (flag > 0) {
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
