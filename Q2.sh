#!/bin/bash

# Create directory with 100 files of 1GB each
echo "Creating directory with 100 files of 1GB each..."
mkdir -p dir1
time for i in {1..100}; do dd if=/dev/zero of=dir1/file$i bs=1M count=1000; done
echo "Directory creation complete."

# Create directory with 10000 files of 10MB each
echo "Creating directory with 10000 files of 10MB each..."
mkdir -p dir2
time for i in {1..10000}; do dd if=/dev/zero of=dir2/file$i bs=1M count=10; done
echo "Directory creation complete."

mkdir dir3

cd dir3

for i in {1..100}; do

    mkdir "subdir$i"

    cd "subdir$i"
    for j in {1..100}; do
        dd if=/dev/urandom of="file$j.txt" bs=10M count=1
    done

    
done
echo "Directory creation complete."

echo "Script execution complete."
