CC=gcc
CFLAGS= -Wall -Werror -pedantic -g

all: ep3opcl

ep3opcl: ep3opcl.o libopcl.o
	$(CC) $(CFLAGS) $^ -lOpenCL -o $@

ep3opcl.o: ep3opcl.c
	$(CC) $(CFLAGS) -c ep3opcl.c -o $@

libopcl.o: ../libopcl.c ../libopcl.h
	$(CC) $(CFLAGS) -c ../libopcl.c -o $@

clean:
	rm ep3opcl ep3opcl.o libopcl.o

