CC=gcc

all: monteCarloGsl

monteCarloGsl: monteCarloGsl.c
	$(CC) -o monteCarloGsl $^ -lgsl -lgslcblas -lm

clean:
	rm -f *.o
