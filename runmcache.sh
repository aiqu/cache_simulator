#!/bin/bash

#echo "--------------------Normal mcache---------------------"
#./mcache astar 64 1 512 64 1 512 64 2 2048
#./mcache astar 128 1 256 128 1 256 64 2 2048
#./mcache astar 256 1 128 256 1 128 64 2 2048
#./mcache astar 64 1 512 64 1 512 128 2 1024
#./mcache astar 128 1 256 128 1 256 128 2 1024
#./mcache astar 256 1 128 256 1 128 128 2 1024
#./mcache astar 64 1 512 64 1 512 256 2 512
#./mcache astar 128 1 256 128 1 256 256 2 512
#./mcache astar 256 1 128 256 1 128 256 2 512
#./mcache astar 64 1 512 64 1 512 64 4 1024
#./mcache astar 128 1 256 128 1 256 64 4 1024
#./mcache astar 256 1 128 256 1 128 64 4 1024
#./mcache astar 64 1 512 64 1 512 128 4 512
#./mcache astar 128 1 256 128 1 256 128 4 512
#./mcache astar 256 1 128 256 1 128 128 4 512
#./mcache astar 64 1 512 64 1 512 256 4 256
#./mcache astar 128 1 256 128 1 256 256 4 256
#./mcache astar 256 1 128 256 1 128 256 4 256
#./mcache astar 64 1 512 64 1 512 64 2 4096
#./mcache astar 128 1 256 128 1 256 64 2 4096
#./mcache astar 256 1 128 256 1 128 64 2 4096
#./mcache astar 64 1 512 64 1 512 128 2 2048
#./mcache astar 128 1 256 128 1 256 128 2 2048
#./mcache astar 256 1 128 256 1 128 128 2 2048
#./mcache astar 64 1 512 64 1 512 256 2 1024
#./mcache astar 128 1 256 128 1 256 256 2 1024
#./mcache astar 256 1 128 256 1 128 256 2 1024
#./mcache astar 64 1 512 64 1 512 64 4 2048
#./mcache astar 128 1 256 128 1 256 64 4 2048
#./mcache astar 256 1 128 256 1 128 64 4 2048
#./mcache astar 64 1 512 64 1 512 128 4 1024
#./mcache astar 128 1 256 128 1 256 128 4 1024
#./mcache astar 256 1 128 256 1 128 128 4 1024
#./mcache astar 64 1 512 64 1 512 256 4 512
#./mcache astar 128 1 256 128 1 256 256 4 512
#./mcache astar 256 1 128 256 1 128 256 4 512

echo "--------------------mcache w/ stream---------------------"
./mcache_stream astar 64 1 512 64 1 512 64 2 2048 1 4
./mcache_stream astar 128 1 256 128 1 256 64 2 2048 1 4
./mcache_stream astar 256 1 128 256 1 128 64 2 2048 1 4
./mcache_stream astar 64 1 512 64 1 512 128 2 1024 1 4
./mcache_stream astar 128 1 256 128 1 256 128 2 1024 1 4
./mcache_stream astar 256 1 128 256 1 128 128 2 1024 1 4
./mcache_stream astar 64 1 512 64 1 512 256 2 512 1 4
./mcache_stream astar 128 1 256 128 1 256 256 2 512 1 4
./mcache_stream astar 256 1 128 256 1 128 256 2 512 1 4
./mcache_stream astar 64 1 512 64 1 512 64 4 1024 1 4
./mcache_stream astar 128 1 256 128 1 256 64 4 1024 1 4
./mcache_stream astar 256 1 128 256 1 128 64 4 1024 1 4
./mcache_stream astar 64 1 512 64 1 512 128 4 512 1 4
./mcache_stream astar 128 1 256 128 1 256 128 4 512 1 4
./mcache_stream astar 256 1 128 256 1 128 128 4 512 1 4
./mcache_stream astar 64 1 512 64 1 512 256 4 256 1 4
./mcache_stream astar 128 1 256 128 1 256 256 4 256 1 4
./mcache_stream astar 256 1 128 256 1 128 256 4 256 1 4
./mcache_stream astar 64 1 512 64 1 512 64 2 4096 1 4
./mcache_stream astar 128 1 256 128 1 256 64 2 4096 1 4
./mcache_stream astar 256 1 128 256 1 128 64 2 4096 1 4
./mcache_stream astar 64 1 512 64 1 512 128 2 2048 1 4
./mcache_stream astar 128 1 256 128 1 256 128 2 2048 1 4
./mcache_stream astar 256 1 128 256 1 128 128 2 2048 1 4
./mcache_stream astar 64 1 512 64 1 512 256 2 1024 1 4
./mcache_stream astar 128 1 256 128 1 256 256 2 1024 1 4
./mcache_stream astar 256 1 128 256 1 128 256 2 1024 1 4
./mcache_stream astar 64 1 512 64 1 512 64 4 2048 1 4
./mcache_stream astar 128 1 256 128 1 256 64 4 2048 1 4
./mcache_stream astar 256 1 128 256 1 128 64 4 2048 1 4
./mcache_stream astar 64 1 512 64 1 512 128 4 1024 1 4
./mcache_stream astar 128 1 256 128 1 256 128 4 1024 1 4
./mcache_stream astar 256 1 128 256 1 128 128 4 1024 1 4
./mcache_stream astar 64 1 512 64 1 512 256 4 512 1 4
./mcache_stream astar 128 1 256 128 1 256 256 4 512 1 4
./mcache_stream astar 256 1 128 256 1 128 256 4 512 1 4

echo "--------------------mcache w/ victim---------------------"
./mcache_victim astar 64 1 512 64 1 512 64 2 2048 1 4
./mcache_victim astar 128 1 256 128 1 256 64 2 2048 1 4
./mcache_victim astar 256 1 128 256 1 128 64 2 2048 1 4
./mcache_victim astar 64 1 512 64 1 512 128 2 1024 1 4
./mcache_victim astar 128 1 256 128 1 256 128 2 1024 1 4
./mcache_victim astar 256 1 128 256 1 128 128 2 1024 1 4
./mcache_victim astar 64 1 512 64 1 512 256 2 512 1 4
./mcache_victim astar 128 1 256 128 1 256 256 2 512 1 4
./mcache_victim astar 256 1 128 256 1 128 256 2 512 1 4
./mcache_victim astar 64 1 512 64 1 512 64 4 1024 1 4
./mcache_victim astar 128 1 256 128 1 256 64 4 1024 1 4
./mcache_victim astar 256 1 128 256 1 128 64 4 1024 1 4
./mcache_victim astar 64 1 512 64 1 512 128 4 512 1 4
./mcache_victim astar 128 1 256 128 1 256 128 4 512 1 4
./mcache_victim astar 256 1 128 256 1 128 128 4 512 1 4
./mcache_victim astar 64 1 512 64 1 512 256 4 256 1 4
./mcache_victim astar 128 1 256 128 1 256 256 4 256 1 4
./mcache_victim astar 256 1 128 256 1 128 256 4 256 1 4
./mcache_victim astar 64 1 512 64 1 512 64 2 4096 1 4
./mcache_victim astar 128 1 256 128 1 256 64 2 4096 1 4
./mcache_victim astar 256 1 128 256 1 128 64 2 4096 1 4
./mcache_victim astar 64 1 512 64 1 512 128 2 2048 1 4
./mcache_victim astar 128 1 256 128 1 256 128 2 2048 1 4
./mcache_victim astar 256 1 128 256 1 128 128 2 2048 1 4
./mcache_victim astar 64 1 512 64 1 512 256 2 1024 1 4
./mcache_victim astar 128 1 256 128 1 256 256 2 1024 1 4
./mcache_victim astar 256 1 128 256 1 128 256 2 1024 1 4
./mcache_victim astar 64 1 512 64 1 512 64 4 2048 1 4
./mcache_victim astar 128 1 256 128 1 256 64 4 2048 1 4
./mcache_victim astar 256 1 128 256 1 128 64 4 2048 1 4
./mcache_victim astar 64 1 512 64 1 512 128 4 1024 1 4
./mcache_victim astar 128 1 256 128 1 256 128 4 1024 1 4
./mcache_victim astar 256 1 128 256 1 128 128 4 1024 1 4
./mcache_victim astar 64 1 512 64 1 512 256 4 512 1 4
./mcache_victim astar 128 1 256 128 1 256 256 4 512 1 4
./mcache_victim astar 256 1 128 256 1 128 256 4 512 1 4

echo "--------------------mcache w/ both---------------------"
./mcache_full astar 64 1 512 64 1 512 64 2 2048 1 4
./mcache_full astar 128 1 256 128 1 256 64 2 2048 1 4
./mcache_full astar 256 1 128 256 1 128 64 2 2048 1 4
./mcache_full astar 64 1 512 64 1 512 128 2 1024 1 4
./mcache_full astar 128 1 256 128 1 256 128 2 1024 1 4
./mcache_full astar 256 1 128 256 1 128 128 2 1024 1 4
./mcache_full astar 64 1 512 64 1 512 256 2 512 1 4
./mcache_full astar 128 1 256 128 1 256 256 2 512 1 4
./mcache_full astar 256 1 128 256 1 128 256 2 512 1 4
./mcache_full astar 64 1 512 64 1 512 64 4 1024 1 4
./mcache_full astar 128 1 256 128 1 256 64 4 1024 1 4
./mcache_full astar 256 1 128 256 1 128 64 4 1024 1 4
./mcache_full astar 64 1 512 64 1 512 128 4 512 1 4
./mcache_full astar 128 1 256 128 1 256 128 4 512 1 4
./mcache_full astar 256 1 128 256 1 128 128 4 512 1 4
./mcache_full astar 64 1 512 64 1 512 256 4 256 1 4
./mcache_full astar 128 1 256 128 1 256 256 4 256 1 4
./mcache_full astar 256 1 128 256 1 128 256 4 256 1 4
./mcache_full astar 64 1 512 64 1 512 64 2 4096 1 4
./mcache_full astar 128 1 256 128 1 256 64 2 4096 1 4
./mcache_full astar 256 1 128 256 1 128 64 2 4096 1 4
./mcache_full astar 64 1 512 64 1 512 128 2 2048 1 4
./mcache_full astar 128 1 256 128 1 256 128 2 2048 1 4
./mcache_full astar 256 1 128 256 1 128 128 2 2048 1 4
./mcache_full astar 64 1 512 64 1 512 256 2 1024 1 4
./mcache_full astar 128 1 256 128 1 256 256 2 1024 1 4
./mcache_full astar 256 1 128 256 1 128 256 2 1024 1 4
./mcache_full astar 64 1 512 64 1 512 64 4 2048 1 4
./mcache_full astar 128 1 256 128 1 256 64 4 2048 1 4
./mcache_full astar 256 1 128 256 1 128 64 4 2048 1 4
./mcache_full astar 64 1 512 64 1 512 128 4 1024 1 4
./mcache_full astar 128 1 256 128 1 256 128 4 1024 1 4
./mcache_full astar 256 1 128 256 1 128 128 4 1024 1 4
./mcache_full astar 64 1 512 64 1 512 256 4 512 1 4
./mcache_full astar 128 1 256 128 1 256 256 4 512 1 4
./mcache_full astar 256 1 128 256 1 128 256 4 512 1 4

echo "***********************************bzip2********************************************"

#echo "--------------------Normal mcache---------------------"
#./mcache bzip2 64 1 512 64 1 512 64 2 2048
#./mcache bzip2 128 1 256 128 1 256 64 2 2048
#./mcache bzip2 256 1 128 256 1 128 64 2 2048
#./mcache bzip2 64 1 512 64 1 512 128 2 1024
#./mcache bzip2 128 1 256 128 1 256 128 2 1024
#./mcache bzip2 256 1 128 256 1 128 128 2 1024
#./mcache bzip2 64 1 512 64 1 512 256 2 512
#./mcache bzip2 128 1 256 128 1 256 256 2 512
#./mcache bzip2 256 1 128 256 1 128 256 2 512
#./mcache bzip2 64 1 512 64 1 512 64 4 1024
#./mcache bzip2 128 1 256 128 1 256 64 4 1024
#./mcache bzip2 256 1 128 256 1 128 64 4 1024
#./mcache bzip2 64 1 512 64 1 512 128 4 512
#./mcache bzip2 128 1 256 128 1 256 128 4 512
#./mcache bzip2 256 1 128 256 1 128 128 4 512
#./mcache bzip2 64 1 512 64 1 512 256 4 256
#./mcache bzip2 128 1 256 128 1 256 256 4 256
#./mcache bzip2 256 1 128 256 1 128 256 4 256
#./mcache bzip2 64 1 512 64 1 512 64 2 4096
#./mcache bzip2 128 1 256 128 1 256 64 2 4096
#./mcache bzip2 256 1 128 256 1 128 64 2 4096
#./mcache bzip2 64 1 512 64 1 512 128 2 2048
#./mcache bzip2 128 1 256 128 1 256 128 2 2048
#./mcache bzip2 256 1 128 256 1 128 128 2 2048
#./mcache bzip2 64 1 512 64 1 512 256 2 1024
#./mcache bzip2 128 1 256 128 1 256 256 2 1024
#./mcache bzip2 256 1 128 256 1 128 256 2 1024
#./mcache bzip2 64 1 512 64 1 512 64 4 2048
#./mcache bzip2 128 1 256 128 1 256 64 4 2048
#./mcache bzip2 256 1 128 256 1 128 64 4 2048
#./mcache bzip2 64 1 512 64 1 512 128 4 1024
#./mcache bzip2 128 1 256 128 1 256 128 4 1024
#./mcache bzip2 256 1 128 256 1 128 128 4 1024
#./mcache bzip2 64 1 512 64 1 512 256 4 512
#./mcache bzip2 128 1 256 128 1 256 256 4 512
#./mcache bzip2 256 1 128 256 1 128 256 4 512

echo "--------------------mcache w/ stream---------------------"
./mcache_stream bzip2 64 1 512 64 1 512 64 2 2048 1 4
./mcache_stream bzip2 128 1 256 128 1 256 64 2 2048 1 4
./mcache_stream bzip2 256 1 128 256 1 128 64 2 2048 1 4
./mcache_stream bzip2 64 1 512 64 1 512 128 2 1024 1 4
./mcache_stream bzip2 128 1 256 128 1 256 128 2 1024 1 4
./mcache_stream bzip2 256 1 128 256 1 128 128 2 1024 1 4
./mcache_stream bzip2 64 1 512 64 1 512 256 2 512 1 4
./mcache_stream bzip2 128 1 256 128 1 256 256 2 512 1 4
./mcache_stream bzip2 256 1 128 256 1 128 256 2 512 1 4
./mcache_stream bzip2 64 1 512 64 1 512 64 4 1024 1 4
./mcache_stream bzip2 128 1 256 128 1 256 64 4 1024 1 4
./mcache_stream bzip2 256 1 128 256 1 128 64 4 1024 1 4
./mcache_stream bzip2 64 1 512 64 1 512 128 4 512 1 4
./mcache_stream bzip2 128 1 256 128 1 256 128 4 512 1 4
./mcache_stream bzip2 256 1 128 256 1 128 128 4 512 1 4
./mcache_stream bzip2 64 1 512 64 1 512 256 4 256 1 4
./mcache_stream bzip2 128 1 256 128 1 256 256 4 256 1 4
./mcache_stream bzip2 256 1 128 256 1 128 256 4 256 1 4
./mcache_stream bzip2 64 1 512 64 1 512 64 2 4096 1 4
./mcache_stream bzip2 128 1 256 128 1 256 64 2 4096 1 4
./mcache_stream bzip2 256 1 128 256 1 128 64 2 4096 1 4
./mcache_stream bzip2 64 1 512 64 1 512 128 2 2048 1 4
./mcache_stream bzip2 128 1 256 128 1 256 128 2 2048 1 4
./mcache_stream bzip2 256 1 128 256 1 128 128 2 2048 1 4
./mcache_stream bzip2 64 1 512 64 1 512 256 2 1024 1 4
./mcache_stream bzip2 128 1 256 128 1 256 256 2 1024 1 4
./mcache_stream bzip2 256 1 128 256 1 128 256 2 1024 1 4
./mcache_stream bzip2 64 1 512 64 1 512 64 4 2048 1 4
./mcache_stream bzip2 128 1 256 128 1 256 64 4 2048 1 4
./mcache_stream bzip2 256 1 128 256 1 128 64 4 2048 1 4
./mcache_stream bzip2 64 1 512 64 1 512 128 4 1024 1 4
./mcache_stream bzip2 128 1 256 128 1 256 128 4 1024 1 4
./mcache_stream bzip2 256 1 128 256 1 128 128 4 1024 1 4
./mcache_stream bzip2 64 1 512 64 1 512 256 4 512 1 4
./mcache_stream bzip2 128 1 256 128 1 256 256 4 512 1 4
./mcache_stream bzip2 256 1 128 256 1 128 256 4 512 1 4

echo "--------------------mcache w/ victim---------------------"
./mcache_victim bzip2 64 1 512 64 1 512 64 2 2048 1 4
./mcache_victim bzip2 128 1 256 128 1 256 64 2 2048 1 4
./mcache_victim bzip2 256 1 128 256 1 128 64 2 2048 1 4
./mcache_victim bzip2 64 1 512 64 1 512 128 2 1024 1 4
./mcache_victim bzip2 128 1 256 128 1 256 128 2 1024 1 4
./mcache_victim bzip2 256 1 128 256 1 128 128 2 1024 1 4
./mcache_victim bzip2 64 1 512 64 1 512 256 2 512 1 4
./mcache_victim bzip2 128 1 256 128 1 256 256 2 512 1 4
./mcache_victim bzip2 256 1 128 256 1 128 256 2 512 1 4
./mcache_victim bzip2 64 1 512 64 1 512 64 4 1024 1 4
./mcache_victim bzip2 128 1 256 128 1 256 64 4 1024 1 4
./mcache_victim bzip2 256 1 128 256 1 128 64 4 1024 1 4
./mcache_victim bzip2 64 1 512 64 1 512 128 4 512 1 4
./mcache_victim bzip2 128 1 256 128 1 256 128 4 512 1 4
./mcache_victim bzip2 256 1 128 256 1 128 128 4 512 1 4
./mcache_victim bzip2 64 1 512 64 1 512 256 4 256 1 4
./mcache_victim bzip2 128 1 256 128 1 256 256 4 256 1 4
./mcache_victim bzip2 256 1 128 256 1 128 256 4 256 1 4
./mcache_victim bzip2 64 1 512 64 1 512 64 2 4096 1 4
./mcache_victim bzip2 128 1 256 128 1 256 64 2 4096 1 4
./mcache_victim bzip2 256 1 128 256 1 128 64 2 4096 1 4
./mcache_victim bzip2 64 1 512 64 1 512 128 2 2048 1 4
./mcache_victim bzip2 128 1 256 128 1 256 128 2 2048 1 4
./mcache_victim bzip2 256 1 128 256 1 128 128 2 2048 1 4
./mcache_victim bzip2 64 1 512 64 1 512 256 2 1024 1 4
./mcache_victim bzip2 128 1 256 128 1 256 256 2 1024 1 4
./mcache_victim bzip2 256 1 128 256 1 128 256 2 1024 1 4
./mcache_victim bzip2 64 1 512 64 1 512 64 4 2048 1 4
./mcache_victim bzip2 128 1 256 128 1 256 64 4 2048 1 4
./mcache_victim bzip2 256 1 128 256 1 128 64 4 2048 1 4
./mcache_victim bzip2 64 1 512 64 1 512 128 4 1024 1 4
./mcache_victim bzip2 128 1 256 128 1 256 128 4 1024 1 4
./mcache_victim bzip2 256 1 128 256 1 128 128 4 1024 1 4
./mcache_victim bzip2 64 1 512 64 1 512 256 4 512 1 4
./mcache_victim bzip2 128 1 256 128 1 256 256 4 512 1 4
./mcache_victim bzip2 256 1 128 256 1 128 256 4 512 1 4

echo "--------------------mcache w/ both---------------------"
./mcache_full bzip2 64 1 512 64 1 512 64 2 2048 1 4
./mcache_full bzip2 128 1 256 128 1 256 64 2 2048 1 4
./mcache_full bzip2 256 1 128 256 1 128 64 2 2048 1 4
./mcache_full bzip2 64 1 512 64 1 512 128 2 1024 1 4
./mcache_full bzip2 128 1 256 128 1 256 128 2 1024 1 4
./mcache_full bzip2 256 1 128 256 1 128 128 2 1024 1 4
./mcache_full bzip2 64 1 512 64 1 512 256 2 512 1 4
./mcache_full bzip2 128 1 256 128 1 256 256 2 512 1 4
./mcache_full bzip2 256 1 128 256 1 128 256 2 512 1 4
./mcache_full bzip2 64 1 512 64 1 512 64 4 1024 1 4
./mcache_full bzip2 128 1 256 128 1 256 64 4 1024 1 4
./mcache_full bzip2 256 1 128 256 1 128 64 4 1024 1 4
./mcache_full bzip2 64 1 512 64 1 512 128 4 512 1 4
./mcache_full bzip2 128 1 256 128 1 256 128 4 512 1 4
./mcache_full bzip2 256 1 128 256 1 128 128 4 512 1 4
./mcache_full bzip2 64 1 512 64 1 512 256 4 256 1 4
./mcache_full bzip2 128 1 256 128 1 256 256 4 256 1 4
./mcache_full bzip2 256 1 128 256 1 128 256 4 256 1 4
./mcache_full bzip2 64 1 512 64 1 512 64 2 4096 1 4
./mcache_full bzip2 128 1 256 128 1 256 64 2 4096 1 4
./mcache_full bzip2 256 1 128 256 1 128 64 2 4096 1 4
./mcache_full bzip2 64 1 512 64 1 512 128 2 2048 1 4
./mcache_full bzip2 128 1 256 128 1 256 128 2 2048 1 4
./mcache_full bzip2 256 1 128 256 1 128 128 2 2048 1 4
./mcache_full bzip2 64 1 512 64 1 512 256 2 1024 1 4
./mcache_full bzip2 128 1 256 128 1 256 256 2 1024 1 4
./mcache_full bzip2 256 1 128 256 1 128 256 2 1024 1 4
./mcache_full bzip2 64 1 512 64 1 512 64 4 2048 1 4
./mcache_full bzip2 128 1 256 128 1 256 64 4 2048 1 4
./mcache_full bzip2 256 1 128 256 1 128 64 4 2048 1 4
./mcache_full bzip2 64 1 512 64 1 512 128 4 1024 1 4
./mcache_full bzip2 128 1 256 128 1 256 128 4 1024 1 4
./mcache_full bzip2 256 1 128 256 1 128 128 4 1024 1 4
./mcache_full bzip2 64 1 512 64 1 512 256 4 512 1 4
./mcache_full bzip2 128 1 256 128 1 256 256 4 512 1 4
./mcache_full bzip2 256 1 128 256 1 128 256 4 512 1 4
