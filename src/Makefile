# 
#  PEDERSEN Ny Aina
#  license: Unlicense
# 

CC := gcc
IDIR := include
OFLAGS := -Ofast -finline-functions -ftree-vectorize -march=native
CFLAGS := -Wall -g -Werror -pedantic -fopenmp -I$(IDIR) $(OFLAGS)
# LDFLAGS := -fsanitize=address -pg
EXE := tsp

all: $(EXE)

tsp: tsp.c reader.o bb.o heap.o prim.o prim_heap.o

%.o: %.c $(IDIR)/%.h $(IDIR)/common.h
	$(CC) $(CFLAGS) -I$(IDIR) -c -o $@ $<

.PHONY: all clean clear

clean:
	rm -f *.o

clear: clean
	rm -f $(EXE)
