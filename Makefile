CC=gcc
# Reduce optimisations to ease understanding of code
CFLAGS=-std=gnu11 -W -Wall -O1 -Wno-unused-parameter -Wno-unused-variable
LDFLAGS=-lm
TARGETS=base arrays

all: $(TARGETS) FORCE

%.o: %.c api.h utils.h
	gcc $(CFLAGS) -S -masm=intel -c -o $@.s $< # generate the assembly file
	gcc $(CFLAGS) -c -o $@ $<

base: main.o base.o 
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

arrays: main.o arrays.o
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	-rm *.o
	-rm $(TARGETS)
	-rm *.o.s

FORCE:
