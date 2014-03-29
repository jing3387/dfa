CC = clang
CFLAGS = -g -std=c99
LDFLAGS = -lcgraph -lgvc

dfa: dfa.o

svg: dfa 1.dot.svg 2.dot.svg

%.dot.svg: %.dot
	./dfa -v $< > $@

clean:
	rm -f dfa *.o *.svg

.PHONY: clean svg
