CC=gcc

all: fft

fft: fft.c
	$(CC) -o fft $^ -lgsl -lgslcblas -lm

clean:
	rm -f *.o
