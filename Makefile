CC = clang
CFLAGS = -g -std=c99
LDFLAGS = -lcgraph -lgvc

dfa: dfa.o

clean:
	rm -f *.o dfa

.PHONY: clean
