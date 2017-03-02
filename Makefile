CC = gcc
CFLAGS = -std=c11

all: sol

sol: sol.c sol.h
	$(CC) $(CFLAGS) $< -o $@

run:: sol
	./$<
