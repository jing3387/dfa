CC = clang
CFLAGS = -g -std=c99
LDFLAGS = -lcgraph -lgvc

dfa: dfa.o

svg: dfa M1.dot.svg M2.dot.svg M3.dot.svg

%.dot.svg: %.dot
	./dfa -v $< > $@

clean:
	rm -f dfa *.o *.svg

.PHONY: clean svg
