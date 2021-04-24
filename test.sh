#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
RUNNING_DIR="$DIR/output"
INPUTS_DIR="$DIR/sample_inputs"

cd "$RUNNING_DIR"
rm -rf *

g++ -o sudoku_solver "$DIR/sudoku_solver.cpp"

# ./sudoku_solver ./sample_inputs/input1.txt

for input in "$INPUTS_DIR"/*.txt; do
	./sudoku_solver "$input"
done

