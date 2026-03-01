#!/bin/bash

# Variables
gap=0.02
width=500
height=250
phi=0.74

# Data generation (Uncomment to generate a fresh base state)
#./bin/sim_engine -g -phi $phi -x $width -y $height -o base_state.csv

echo "Starting force chain threshold analysis..."

# Loop from 1.000 down to 0.800. We use -$gap because we are counting backwards!
for f in $(seq -f "%.3f" 1.000 -$gap 0.600); do
    echo "=========================================="
    echo "Processing Force Factor: $f"

    # 1. Connectivity scan
    ./bin/sim_engine -i base_state.csv -f $f -o "network_${f}.csv"

    # 2. Plot the results
    ./bin/plotter -i "network_${f}.csv" -x $width -y $height -o "network_${f}.svg"

done

echo "=========================================="
echo "All scans and plots completed successfully!"