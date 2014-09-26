#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "stb_image_ext.h"

int main(__unused int argc, __unused char* argv[])
{
    t_img_desc img = load_image("test1.jpg", 1);

    printf("Image: %ix%i\n", img.x, img.y);
    printf("Comp: ");

    switch (img.comp) {
    case 1:
        printf("grey\n");
        break;
    case 2:
        printf("grey, alpha\n");
        break;
    case 3:
        printf("red, green, blue\n");
        break;
    default:
        printf("red, green, blue, alpha\n");
        break;
    }

    free_image(&img);
    return EXIT_SUCCESS;
}
