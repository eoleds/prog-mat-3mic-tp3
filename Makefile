CC=gcc
# Reduce optimisations to ease understanding of code
CFLAGS=-std=gnu11 -W -Wall -O1 -mavx2 -Wno-unused-parameter -Wno-unused-variable -lpthread 
LDFLAGS=-lm -pthread
TARGETS=arrays

all: $(TARGETS) FORCE

%.o: %.c api.h utils.h simd.h
	gcc $(CFLAGS) -S -masm=intel -c -o $@.s $< # generate the assembly file
	gcc $(CFLAGS) -c -o $@ $<

arrays: main.o arrays.o
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

simd-test: simd-test.c FORCE
	gcc $(CFLAGS) simd-test.c -o simd-test
	./simd-test

clean:
	-rm *.o
	-rm $(TARGETS)
	-rm *.o.s
	-rm simd-test

FORCE:
