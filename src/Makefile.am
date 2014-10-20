SUBDIRS = stb_image
bin_PROGRAMS = ocr test
AM_CFLAGS = -Wall -Wextra -Werror -std=c99 -O3

ocr_SOURCES = main.c cmdline.c
ocr_LDADD = stb_image/lib_stb_image.la lib_processing.la
BUILT_SOURCES = cmdline.c
cmdline.c: ocr.ggo
	gengetopt < $(srcdir)/ocr.ggo

test_SOURCES = test.c benchmark.c
test_LDADD = stb_image/lib_stb_image.la lib_processing.la

noinst_LTLIBRARIES = lib_processing.la
lib_processing_la_SOURCES = preprocessing.c detection.c
lib_processing_la_LIBADD = -lm
