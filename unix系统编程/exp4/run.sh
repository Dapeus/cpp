#!/bin/bash
cd /mnt/d/code/cpp/unix系统编程/exp4/
if [ ! -d "./server" ]; then
    make
fi
./server