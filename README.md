# DFA
A Deterministic Finite Automata simulator using the DOT language and Graphviz
libraries.

## Features
* Describe DFAs using the DOT language
* Run a DFA across strings in an interactive prompt with tracing
* Visualise the DFA as an SVG

## Usage
./dfa [-vt?] file

* v: output an SVG to stdout
* t: test a DFA by only printing valid strings
* file: If not -v, file should be a DFA

## Files
* dfa.c: main source
* 1.dot: An NFA for the regular expression (aba)*b|(b*a)*; in a dot file for
visualisation purposes
* 2.dot: A DFA for the NFA in 1.dot
* 3.dot: A minimised version of the DFA in 2.dot

## Testing
* Input strings generated with the [regular expression grammar language dictionary
generator](http://regldg.com/) using the regular expression (a|b)*
* Expected output created using the command:
`$ egrep -w '(aba)*b|(b*a)*' tests/in > tests/expected`
