#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "test.h"
#include "minunit.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_ext.h"
#include "benchmark.h"

#define PRINT_LINE printf("-----------------------------------\n")

int tests_run = 0;

int main()
{
    printf("\nStart test...\n");
    PRINT_LINE;
    print_limits();
    PRINT_LINE;

    char* result = all_tests();

    if (result) {
        printf("At least one test failed:\n");
        printf("    %s\n", result);
    } else {
        printf("All tests passed\n");
    }

    PRINT_LINE;
    printf("Tests run: %d\n\n", tests_run);

    return (!result) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void print_limits()
{
    printf("%9s = %i\n", "CHAR_MIN", CHAR_MIN);
    printf("%9s = %i\n", "CHAR_MAX", CHAR_MAX);
    printf("%9s = %i\n", "UCHAR_MAX", UCHAR_MAX);

    printf("%9s = %i\n", "SHORT_MIN", SHRT_MIN);
    printf("%9s = %i\n", "SHORT_MAX", SHRT_MAX);

    printf("%9s = %i\n", "INT_MIN", INT_MIN);
    printf("%9s = %i\n", "INT_MAX", INT_MAX);

    printf("%9s = %ld\n", "LONG_MIN", LONG_MIN);
    printf("%9s = %ld\n", "LONG_MAX", LONG_MAX);
}

char* all_tests()
{
    mu_run_test(test_load_image);
    PRINT_LINE;
    mu_run_test(test_greyscale);
    PRINT_LINE;
    mu_run_test(test_histogram);
    PRINT_LINE;
    return 0;
}

char* test_load_image()
{
    int x = 0, y = 0, comp = 0;

    char filename[][10] = {
        "test1.jpg",
        "test2.png",
        "test3.bmp",
    };

    char *error = malloc(sizeof(char) * 50);

    for (int i = 0; i < 3; i++) {
        sprintf(error, "failed to load image %s", filename[i]);
        mu_assert(error, stbi_info(filename[i], &x, &y, &comp) == 1);

        printf("%10s: %ix%i (%i)\n", filename[i], x, y, comp);
    }

    free(error);
    return 0;
}

char* test_greyscale()
{
    t_img_desc *img;

    char filename[][50] = {
        "greyscale1.jpg",
        "greyscale2.jpg",
        "greyscale3.jpg",
        "greyscale4.png"
    };

    char *error = malloc(sizeof(char) * 150);
    char *out = malloc(sizeof(char) * 100);

    for (int i = 0; i < 4; i++) {
        img = load_image(filename[i], 3);
        grey_scale(img);

        sprintf(out, "out_greyscale%i.png", i + 1);
        sprintf(error, "failed to write %s", out);

        int result = stbi_write_png(out, img->x, img->y, img->comp,
                img->data, img->x);
        mu_assert(error, result != 0);

        printf("write: %s\n", out);
        free_image(img);
    }

    free(error);
    free(out);
    return 0;
}

char* test_histogram()
{
    t_img_desc *img;

    char filename[][50] = {
        "greyscale1.jpg",
        "greyscale2.jpg",
        "greyscale3.jpg",
        "greyscale4.png"
    };

    char *error = malloc(sizeof(char) * 150);

    printf("Histogram bench:\n\n");

    for (int i = 0; i < 4; i++) {
        img = load_image(filename[i], 3);
        grey_scale(img);

        bench_start();
        uint* h_fast = histogram_fast(img);
        bench_end();

        printf("%s:\n", filename[i]);
        printf("\t");
        bench_time();
        printf("\t");
        bench_write_mem(sizeof(char) * img->x * img->y * img->comp);

        uint* h = histogram(img);
        int n_error = 0;

        for (int j = 0; j < 256; j++) {
            if (h[i] != h_fast[i])
                n_error++;
        }

        sprintf(error, "%d differences between the two histograms", n_error);
        mu_assert(error, n_error == 0);

        free(h_fast);
        free_image(img);
    }

    return 0;
}
