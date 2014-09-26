#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "test.h"
#include "minunit.h"
#include "stb_image.h"

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

    char* error = malloc(sizeof(char) * 50);

    for (int i = 0; i < 3; i++) {
        sprintf(error, "failed to load image %s", filename[i]);
        mu_assert(error, stbi_info(filename[i], &x, &y, &comp) == 1);

        printf("%10s: %ix%i (%i)\n", filename[i], x, y, comp);
    }

    return 0;
}
