CC=gcc
LD=$(CC)
CFLAGS=-std=c99 -Wall -Wextra -Werror -O3
LDLIBS=-lm

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
	$(LD) -o $@ $^ $(LDLIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf ocr out_img* $(BUILD_DIR)
