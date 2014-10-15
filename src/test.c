#include "test.h"

#define PRINT_LINE printf("-----------------------------------\n")

int tests_run = 0;

const int NFILES = 7;
char FILENAME[][50] = {
    "test1.jpg",
    "test2.jpg",
    "test3.jpg",
    "test4.png",
    "test5.jpg",
    "test6.jpg",
    "test7.jpg"
};

char *error;
char *out;
t_img_desc **img;

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
    error = malloc(sizeof(char) * 200);
    out = malloc(sizeof(char) * 100);
    img = malloc(sizeof(t_img_desc*) * NFILES);

    if (!error || !out || !img)
        exit(EXIT_FAILURE);

    mu_run_test(test_load_image);
    PRINT_LINE;
    mu_run_test(test_average_filter);
    PRINT_LINE;
    mu_run_test(test_greyscale);
    PRINT_LINE;
    //mu_run_test(test_gaussian_blur);
    //PRINT_LINE;
    mu_run_test(test_histogram);
    PRINT_LINE;
    mu_run_test(test_binarize);
    PRINT_LINE;

    for (int i = 0; i < NFILES; i++)
        free_image(img[i]);
    free(img);
    free(error);
    free(out);

    return 0;
}

char* test_load_image()
{
    int x = 0, y = 0, comp = 0;

    for (int i = 0; i < NFILES; i++) {
        sprintf(error, "failed to load image %s", FILENAME[i]);

        mu_assert(error, stbi_info(FILENAME[i], &x, &y, &comp) == 1);
        printf("%10s: %ix%i (%i)\n", FILENAME[i], x, y, comp);

        img[i] = load_image(FILENAME[i], 3);
    }

    return 0;
}

char* test_average_filter()
{
    for (int i = 0; i < NFILES; i++) {
        sprintf(out, "out_averagefilter%i.png", i + 1);
        sprintf(error, "failed to write %s", out);

        average_filter(img[i]);

        int result = write_image(out, img[i]);
        mu_assert(error, result != 0);

        printf("write: %s\n", out);
    }

    return 0;
}

char* test_greyscale()
{
    for (int i = 0; i < NFILES; i++) {
        sprintf(out, "out_greyscale%i.png", i + 1);
        sprintf(error, "failed to write %s", out);

        grey_scale(img[i]);

        int result = write_image(out, img[i]);
        mu_assert(error, result != 0);

        printf("write: %s\n", out);
    }

    return 0;
}

char* test_gaussian_blur()
{
    for (int i = 0; i < NFILES; i++) {
        sprintf(out, "out_gaussianblur%i.png", i + 1);
        sprintf(error, "failed to write %s", out);

        gaussian_blur(img[i], 0.84089642);

        int result = write_image(out, img[i]);
        mu_assert(error, result != 0);

        printf("write: %s\n", out);
    }

    return 0;
}

char* test_histogram()
{
    printf("Histogram bench:\n\n");

    for (int i = 0; i < NFILES; i++) {
        bench_start();
        uint* h_fast = histogram_fast(img[i]);
        bench_end();

        printf("%s:\n", FILENAME[i]);
        printf("\t");
        bench_time();
        printf("\t");
        bench_write_mem(sizeof(char) * img[i]->x * img[i]->y * img[i]->comp);

        uint* h = histogram(img[i]);
        int n_error = 0;

        for (int j = 0; j < 256; j++) {
            if (h[i] != h_fast[i])
                n_error++;
        }

        sprintf(error, "%d differences between the two histograms", n_error);
        mu_assert(error, n_error == 0);

        free(h_fast);
        free(h);
    }

    return 0;
}

char* test_binarize()
{
    for (int i = 0; i < NFILES; i++) {
        sprintf(out, "out_binarize%i.png", i + 1);
        sprintf(error, "failed to write %s", out);

        binarize_otsu(img[i]);
        //binarize_basic(img[i]);

        int result = write_image(out, img[i]);
        mu_assert(error, result != 0);

        printf("write: %s\n", out);
    }

    return 0;
}
