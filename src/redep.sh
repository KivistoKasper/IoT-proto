#!/bin/bash
echo "Pulling latest changes from git repository..."
git pull

echo "Compiling the miner..."
g++ src/miner.cpp -o src/miner.out

echo "Running the miner..."
./src/miner.out