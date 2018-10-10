CC = gcc
CFLAGS = -Wall -g

all: main_cp143a main_lsy143a main_pipe143a sh

main_cp143a: main_cp143a.c
	$(CC) $(CFLAGS) $@.c -o $@

main_lsy143a: main_lsy143a.c
	$(CC) $(CFLAGS) $@.c -o $@

main_pipe143a: main_pipe143a.c
	$(CC) $(CFLAGS) $@.c -o $@

sh: sh.c
	$(CC) $(CFLAGS) $@.c -o $@

# SRC = $(wildcard *.c)
# EXE = $(SRC:.c=)

# all: $(EXE)

# $(EXE): $(SRC)
# 	$(CC) $(CFLAGS) $@.c -o $@
