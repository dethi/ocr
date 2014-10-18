SUBDIRS = stb_image
bin_PROGRAMS = ocr test

ocr_SOURCES = main.c
ocr_LDADD = stb_image/lib_stb_image.la lib_processing.la
ocr_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

test_SOURCES = test.c benchmark.c
test_LDADD = stb_image/lib_stb_image.la lib_processing.la
test_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

noinst_LTLIBRARIES = lib_processing.la
lib_processing_la_SOURCES = preprocessing.c
lib_processing_la_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
lib_processing_la_LIBADD = -lm
