#!/bin/bash

# Initial Variables
width=200
height=100
phi=0.85
f=1.0

echo "Starting linear scale-up analysis..."

# Loop 10 times
for i in {1..10}; do
    echo "=========================================="
    echo "Iteration $i: Width=$width, Height=$height"

    # Create unique filenames for this specific size
    base_csv="base_w${width}_h${height}.csv"
    network_csv="network_w${width}_h${height}.csv"
    network_svg="network_w${width}_h${height}.svg"

    # 1. Generate new particles for the current box size
    echo "Generating and relaxing particles..."
    ./bin/sim_engine -g -phi $phi -x $width -y $height -o $base_csv

    # 2. Connectivity scan (Force factor fixed at 1.0)
    echo "Running DSU connectivity scan..."
    # TIP: We put the 'time' command here so you can see exactly how fast the DSU is!
    time ./bin/sim_engine -i $base_csv -f $f -o $network_csv

    # 3. Plot the results
    echo "Rendering SVG..."
    ./bin/plotter -i $network_csv -w $width -y $height -o $network_svg

    # 4. Add 200 to the width for the next loop
    width=$((width + 200))
done

echo "=========================================="
echo "Linear scale-up analysis completed successfully!"