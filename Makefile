CC=gcc
LD=$(CC)
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -O3
LDLIBS=-lm

# GTK
CFLAGS += `pkg-config gtk+-3.0 --cflags` \
		 `pkg-config gtkspell3-3.0 --cflags` \
		 -fPIC -I. -c -D REENTRANT

LDFLAGS=`pkg-config gtk+-3.0 --libs` \
		`pkg-config gmodule-2.0 --libs` \
		`pkg-config gtkspell3-3.0 --libs`

ifndef
BUILD_DIR = _build
endif

ifeq ($(strip $(BUILD_DIR)),)
BUILD_DIR := _build
endif

SRC=$(shell find src -type f -name "*.c")
OBJ=$(addprefix $(BUILD_DIR)/, $(subst src/,,$(SRC:.c=.o)))

all: ocr

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c -o $@ $^

ocr: $(OBJ) | $(BUILD_DIR)
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf ocr out_img* *.txt *gui.glade~ $(BUILD_DIR)
