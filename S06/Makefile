CC=mpicc

common:
	$(CC) -c common.c

build: common
	$(CC) *.o -o laplace.out laplace.c

build_simple: common
	$(CC) *.o -o simple.out simple.c

all: build
	./laplace.out

clean:
	rm *.out *.o