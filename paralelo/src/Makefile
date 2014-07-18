
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -O2 -fopenmp
DEPS = tiempo.h
BINS = saxpy

all: $(BINS)
	cp -f $(BINS) ../bin/

saxpy: saxpy.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^ -lm


clean:
	rm -f $(BINS) ../bin/*
