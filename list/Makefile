CC := gcc
INCLUDE_DIRS := -I..
CFLAGS := -g -Wall -Wextra -std=c99
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

all: $(OBJ)

%.o: %.c
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)
