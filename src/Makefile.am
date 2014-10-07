bin_PROGRAMS = ocr test

ocr_SOURCES = main.c main.h
ocr_LDADD = lib_stb_image.la
ocr_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

test_SOURCES = test.c test.h minunit.h benchmark.c benchmark.h
test_LDADD = lib_stb_image.la
test_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

noinst_LTLIBRARIES = lib_stb_image.la
lib_stb_image_la_SOURCES = stb_image.c stb_image.h stb_image_write.c stb_image_write.h stb_image_ext.c stb_image_ext.h
lib_stb_image_la_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
lib_stb_image_la_LIBADD = -lm
