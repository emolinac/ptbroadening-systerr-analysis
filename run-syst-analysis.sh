#!/bin/bash

mother_dir=$(pwd)

# Compile software
make clean
make

# Calculate what is necessary
cd ./bin
./calculate-broadening-zh-systerror
./calculate-broadening-a13-systerror
./calculate-broadening-differential-systerror

# Print the plots
./print-plot-broadening-zh
./print-plot-broadening-a13
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzha13
./print-plot-deviations-broadening