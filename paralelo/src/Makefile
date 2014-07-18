
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -O2 -fopenmp
BINS = saxpy integrador

all: $(BINS)
	cp -f $(BINS) ../bin/

saxpy: saxpy.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

integrador: integrador.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f $(BINS) ../bin/*
