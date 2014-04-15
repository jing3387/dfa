CC = clang
CFLAGS = -g -std=c99
LDFLAGS = -lcgraph -lgvc

dfa: dfa.o

test: dfa
	@./test.sh 2.dot
	@./test.sh 3.dot

svg: dfa 1.dot.svg 2.dot.svg 3.dot.svg

%.dot.svg: %.dot
	./dfa -v $< > $@

clean:
	rm -f dfa *.out *.o *.svg

.PHONY: clean svg test
