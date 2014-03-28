# DFA
A Deterministic Finite Automata simulator using the DOT language.

## Usage
./dfa file

## Included machines
* M1.dot: (aba)*b
* M2.dot: (b*a)*
* M.dot: (aba)*b|(b*a)*
* Min.dot: a minimised version of M

## Features
* Describe DFAs using the DOT language
* Process a string using the description of the DFA to find accepted and
  rejected strings of the DFA's language

## The DOT language
Able to be processed by Graphviz tools to create a visualisation of the DFA.
