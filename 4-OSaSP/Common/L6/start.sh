#!/usr/bin/env bash

GENERATOR=./bin/generator
SCANNER=./bin/scanner
SORT=./bin/sort

SIZE=8192
NAME=./bin/db8192

MEMSIZE=4096
SECTIONS=8
THREADS=3

echo "Generate $NAME with size $SIZE"

./$GENERATOR $SIZE $NAME

echo "Scan $NAME before sort"

./$SCANNER $NAME

echo "Sort $NAME with $MEMSIZE memsize, $SECTIONS sections, $THREADS threads"

./$SORT $MEMSIZE $SECTIONS $THREADS $NAME

echo "Scan $NAME after sort"

./$SCANNER $NAME