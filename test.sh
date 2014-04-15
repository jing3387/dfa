#!/bin/bash

out=$(basename $1 .dot).out

./dfa -t $1 < test.in > $out

diff -q test.exp $out

if [ $$? ]; then
	echo "$1 accepted all expected strings"
else
	echo "$1 rejected the following strings:"
	diff test.exp $out
fi
