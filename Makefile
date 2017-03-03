CC = gcc
CFLAGS = -std=c11 -Wall

all: sol

sol: sol.c sol.h getche.h
	$(CC) $(CFLAGS) $< -o $@

run:: sol
	./$<
