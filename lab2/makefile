CC=gcc
GSL_HOME=

CFLAGS=-I$(GSL_HOME)/include
LDFLAGS=-L$(GSL_HOME)/lib -lgsl -lgslcblas -lm

all: dokladnosc

dokladnosc: root_finding.c
	$(CC) $(LDFLAGS) -o root_finding $^

clean:
	rm -f *.o
