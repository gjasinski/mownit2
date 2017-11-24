CC=gcc

all: main

fft: main.c
	$(CC) -o main $^ -lgsl -lgslcblas -lm

clean:
	rm -f *.o
