CC=gcc
GSL_HOME=/usr/local/lib

CFLAGS=-I$(GSL_HOME)/include
LDFLAGS=-L$(GSL_HOME)/lib -lgsl -lgslcblas -lm

all: matrix

matrix: matrix.c
	$(CC) -I/usr/local/include -c matrix.c
	$(CC) -L/usr/local/lib matrix.o -lgsl -lgslcblas -lm -o matrix

clean:
	rm -f *.o
