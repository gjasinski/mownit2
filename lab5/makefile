CC=gcc

all: monteCarloGsl

monteCarloGsl: monteCarloGsl.c
	$(CC) -o monteCarloGsl $^ -lgsl -lgslcblas -lm
	$(CC) hitAndMiss.c -o hitAndMiss -lm

clean:
	rm -f *.o
