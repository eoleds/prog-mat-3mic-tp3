CC=gcc
# Reduce optimisations to ease understanding of code
CFLAGS=-std=gnu11 -W -Wall -O1
LDFLAGS=-lm
TARGETS=base

all: $(TARGETS) FORCE

%.o: %.c api.h
	gcc $(CFLAGS) -S -c -o $@.asm $< # generate the assembly file
	gcc $(CFLAGS) -c -o $@ $<

base: main.o base.o api.h
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

arrays: main.o arrays.o api.h
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	-rm *.o
	-rm $(TARGETS)
	-rm *.o.asm

FORCE:
