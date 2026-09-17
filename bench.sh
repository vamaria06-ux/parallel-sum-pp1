#!/usr/bin/env bash

set -euo pipefail
 
BIN=./sum
OUT=results_raw.csv
> "$OUT"
 
THREADS=(1 2 4 8 16 32 64 128 256 512 1024)
 
for t in "${THREADS[@]}"; do
    for run in 1 2 3 4 5; do
        line=$("$BIN" "$t")
        echo "$line" >> "$OUT"
    done
    echo "threads=$t done" >&2
done
