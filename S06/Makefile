CC=mpicc

common:
	$(CC) -c common.c

build: common
	$(CC) *.o -o laplace.out laplace.c

build_simple: common
	$(CC) *.o -o simple.out simple.c

all: clean build build_simple

clean:
	rm -f *.out *.o