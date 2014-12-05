CC=clang
LD=$(CC)
CFLAGS=-std=c99 -Wall -Wextra -O0 -g

EXEC=testing
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(EXEC): $(OBJ)
	$(LD) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	rm -rf $(EXEC) *.saved *.o
