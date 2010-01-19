export CC = gcc
export CFLAGS = -g -Wall -Wextra -std=c99
export INCLUDE := -I$(CURDIR) -I$(CURDIR)/list
DIRS = list
SRC = $(wildcard *.c list/*.c)
OBJ = $(SRC:.c=.o)
EXEC = test

all: $(DIRS) $(EXEC)

list:
	@(cd list && $(MAKE))

test: $(OBJ)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<

.PHONY: clean $(DIRS)

clean:
	@(cd list && $(MAKE) $@)
	rm -rf $(OBJ) $(EXEC)
