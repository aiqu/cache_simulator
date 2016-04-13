#!/bin/bash

echo "Simulation for astar"
#echo "Direct mapped cache:"
./cache astar  64 1 1024
#echo "N-way set associative cache:"
./cache astar  64 2 512
./cache astar  64 4 256
./cache astar  64 8 128
#echo "Fully associative cache:"
./cache astar  64 1024 1
#echo "Direct mapped cache:"
./cache astar  64 1 2048
#echo "N-way set associative cache:"
./cache astar  64 2 1024
./cache astar  64 4 512
./cache astar  64 8 256
#echo "Fully associative cache:"
./cache astar  64 2048 1
#echo "Direct mapped cache:"
./cache astar  64 1 4096
#echo "N-way set associative cache:"
./cache astar  64 2 2048
./cache astar  64 4 1024
./cache astar  64 8 512
#echo "Fully associative cache:"
./cache astar  64 4096 1
echo "Simulation for bzip2"
#echo "Direct mapped cache:"
./cache bzip2  64 1 1024
#echo "N-way set associative cache:"
./cache bzip2  64 2 512
./cache bzip2  64 4 256
./cache bzip2  64 8 128
#echo "Fully associative cache:"
./cache bzip2  64 1024 1
#echo "Direct mapped cache:"
./cache bzip2  64 1 2048
#echo "N-way set associative cache:"
./cache bzip2  64 2 1024
./cache bzip2  64 4 512
./cache bzip2  64 8 256
#echo "Fully associative cache:"
./cache bzip2  64 2048 1
#echo "Direct mapped cache:"
./cache bzip2  64 1 4096
#echo "N-way set associative cache:"
./cache bzip2  64 2 2048
./cache bzip2  64 4 1024
./cache bzip2  64 8 512
#echo "Fully associative cache:"
./cache bzip2  64 4096 1
