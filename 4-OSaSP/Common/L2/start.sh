#!/usr/bin/env bash

PARENT=parent
CHILD=son
LIST=environment

echo "Running $PARENT..."

CHILD_PATH="$CHILD" ./"$PARENT" "$LIST"