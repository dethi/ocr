#include "main.h"

int main(int argc, char *argv[])
{
    assert(argc > 1);
    make_nn(argv[1], "ocr.saved");
    return EXIT_SUCCESS;
}
