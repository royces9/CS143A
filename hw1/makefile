CC = gcc
CFLAGS = -Wall -g

SRC = $(wildcard *.c)
EXE = $(SRC:.c=)

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $@.c -o $@

.PHONY: clean
clean:
	rm $(EXE)
