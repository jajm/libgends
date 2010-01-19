SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(OBJ)

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)
