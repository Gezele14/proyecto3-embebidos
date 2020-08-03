#!/bin/sh
rm bin/server
make
cd bin
./server 
cd ..
