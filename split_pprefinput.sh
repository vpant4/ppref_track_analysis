#!/bin/bash

# Check if the correct number of arguments is provided
if [ $# -ne 3 ]; then
    echo "Usage: $0 <input_file> <N> <sample_prefix>"
    exit 1
fi

# Date of submission
formatted_date=$(date +"%Y%m%d")

# Input file name
input_file="$1"
# Number of entries for each sublist
N="$2"

# Prefix

sample_prefix="$3"


# Check if the input file exists
if [ ! -f "$input_file" ]; then
    echo "Input file '$input_file' not found."
    exit 1
fi

# Count the total number of lines in the input file
total_lines=$(wc -l < "$input_file")

# Calculate the number of sublists needed
num_sublists=$((total_lines / N))
if [ $((total_lines % N)) -ne 0 ]; then
    ((num_sublists++))
fi

if [ ! -d "$PWD/input/ppref2024/${formatted_date}" ]; then
    #echo "Directory '$PWD/input/pPb8160/${formatted_date}' does not exist. Creating..."
    mkdir -p "$PWD/input/ppref2024/${formatted_date}"
fi

# Create sublists
for ((i = 0; i < num_sublists; i++)); do
    start=$((i * N + 1))  # Calculate start line number for current sublist
    end=$((start + N - 1))  # Calculate end line number for current sublist
    sublist_file="${sample_prefix}_$((i+1)).txt"  # Name of sublist file
    # Extract sublist
    sed -n "${start},${end}p" "$input_file" > "$PWD/input/ppref2024/${formatted_date}/$sublist_file"  
done

echo $num_sublists
