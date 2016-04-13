#!/bin/bash

#./cache astar 64 1 1024 1 4
#./cache astar 64 1 2048 1 4
#./cache astar 64 1 4096 1 4

#echo ""

#./cache astar 64 1 1024 1 8
#./cache astar 64 1 2048 1 8
#./cache astar 64 1 4096 1 8

#echo ""

#./cache bzip2 64 1 1024 1 4
#./cache bzip2 64 1 2048 1 4
#./cache bzip2 64 1 4096 1 4

#echo ""

#./cache bzip2 64 1 1024 1 8
#./cache bzip2 64 1 2048 1 8
#./cache bzip2 64 1 4096 1 8

echo "Victim buffer test 1,2,4,8"
make CFLAGS=-DVICTIMCACHE lru
./cache astar 64 1 2048 1 1
./cache astar 64 1 2048 1 2
./cache astar 64 1 2048 1 4
./cache astar 64 1 2048 1 8
echo ""
./cache bzip2 64 1 2048 1 1
./cache bzip2 64 1 2048 1 2
./cache bzip2 64 1 2048 1 4
./cache bzip2 64 1 2048 1 8
echo ""
echo "Stream buffer test 1,2,4,8"
make CFLAGS=-DSTREAMBUFFER lru
./cache astar 64 1 2048 1
./cache astar 64 1 2048 2
./cache astar 64 1 2048 4
./cache astar 64 1 2048 8
echo ""
./cache bzip2 64 1 2048 1
./cache bzip2 64 1 2048 2
./cache bzip2 64 1 2048 4
./cache bzip2 64 1 2048 8
echo ""
echo "Combined buffer test 1,2,4,8"
make CFLAGS=-DVICTIMCACHE CFLAGS+=-DSTREAMBUFFER lru
./cache astar 64 1 2048 1 1
./cache astar 64 1 2048 1 2
./cache astar 64 1 2048 1 4
./cache astar 64 1 2048 1 8
echo ""
./cache bzip2 64 1 2048 1 1
./cache bzip2 64 1 2048 1 2
./cache bzip2 64 1 2048 1 4
./cache bzip2 64 1 2048 1 8
echo ""
