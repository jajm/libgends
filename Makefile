CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c99
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXEC = test

all: $(EXEC)

test: $(OBJ)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -rf *.o *.log $(EXEC)
