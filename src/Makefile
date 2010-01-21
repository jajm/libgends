export CC = gcc
export CFLAGS = -g -Wall -Wextra -std=c99
export INCLUDES := -I$(CURDIR) -I$(CURDIR)/list
DIRS = list
SRC = $(wildcard *.c $(foreach dir, $(DIRS), $(dir)/*.c))
OBJ = $(SRC:.c=.o)
EXEC = test

all: $(DIRS) $(EXEC)

list:
	@(cd list && $(MAKE))

test: $(OBJ)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<

.PHONY: clean $(DIRS)

clean:
	@(cd list && $(MAKE) $@)
	rm -rf $(OBJ) $(EXEC)
