bin_PROGRAMS = ocr test

ocr_SOURCES = main.c main.h
ocr_LDADD = lib_stb_image.a
ocr_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

test_SOURCES = test.c test.h minunit.h benchmark.c benchmark.h
test_LDADD = lib_stb_image.a
test_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

noinst_LIBRARIES = lib_stb_image.a
lib_stb_image_a_SOURCES = stb_image.c stb_image.h stb_image_write.c stb_image_write.h stb_image_ext.c stb_image_ext.h
lib_stb_image_a_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
