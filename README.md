# DFA
A Deterministic Finite Automata simulator using the DOT language and Graphviz
libraries.

## Features
* Describe DFAs using the DOT language
* Simulate a DFA with user-provided strings in an interactive prompt with
tracing
* Visualise the DFA as an SVG

## Usage
`usage: dfa [-vt?] file`

* v: print an SVG to stdout
* t: test a DFA by only printing accepted strings
* file: If not -v, file should be a DFA

e.g. to simulate the provided minimial DFA: `./dfa 3.dot`

## Files
* dfa: main executable
* dfa.c: main source
* 1.dot: an NFA for the regular expression (aba)*b|(b*a)*; in a dot file for
visualisation purposes
* 2.dot: a DFA for the NFA in 1.dot
* 3.dot: a minimal version of the DFA in 2.dot
* test.in: input strings generated as specified in [Testing]
* test.exp: expected output strings generated as specified in [Testing]
* test.sh: used to run the tests over DFAs as described in [Testing]

## Testing
Conducted by giving the program a set of generated input strings over (a|b)* in
test mode (-t) and redirecting accepted strings to an output file. These
accepted strings and then compared to a file of expected strings which are the
filtered results of a regular expression over the generated input strings.

* Input strings generated with [REGLDG](http://regldg.com/) using the command: `$ ./regldg --debug-code=1 --universe-set=7 --universe-checking=3 --max-length=8 --readable-output "(a|b)*" > test.in`
* Expected output generated using the command: `$ egrep -w '(aba)*b|(b*a)*' test.in > test.exp`
