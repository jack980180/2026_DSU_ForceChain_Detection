#!/bin/bash

# Define the output file for your performance data
perf_csv="performance_results.csv"

# Write the CSV headers
echo "width,num_particles,time_seconds" > "$perf_csv"

echo "Measuring DSU connectivity scan execution time..."
echo "================================================="

# Loop through widths from 200 to 2000 in steps of 200
for width in $(seq 200 200 2000); do
    input_file="results/network_w${width}_h100.csv"

    # Check if the file actually exists before trying to run it
    if [[ ! -f "$input_file" ]]; then
        echo "File $input_file not found, skipping..."
        continue
    fi

    # 1. Extract Particle Count
    # Count the total lines in the CSV and subtract 1 (for the header row)
    # Using 'tr -d' to remove any whitespace 'wc' might add on macOS
    total_lines=$(wc -l < "$input_file" | tr -d ' ')
    particles=$((total_lines - 1))

    # 2. Measure Execution Time
    # We use a subshell {} to capture the 'time' output natively.
    # Normal simulation prints are sent to /dev/null so your console stays clean!
    exec_time=$({ TIMEFORMAT="%R"; time ./bin/sim_engine -i "$input_file" -f 0.8 -o "tmp_out.csv" > /dev/null; } 2>&1)

    # Print to console so you can watch the progress
    echo "Width: $width | Particles: $particles | Time: $exec_time sec"

    # 3. Save to your performance CSV
    echo "$width,$particles,$exec_time" >> "$perf_csv"
done

# Clean up the temporary file we just used for testing
rm -f tmp_out.csv

echo "================================================="
echo "Success! Performance data exported to $perf_csv"