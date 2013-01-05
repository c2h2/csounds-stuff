#!/bin/sh

#sudo apt-get install libsndfile1-dev 
gcc  -I/usr/local/include -O2 -c testsnd.c 
libtool  --mode=link gcc  -g -O2 -std=gnu99 -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Waggregate-return -Wcast-align -Wcast-qual -Wnested-externs -Wshadow -Wpointer-arith -o testsnd ./testsnd.o -L/usr/local/lib -lsndfile   -lpthread -lrt   -lm 

#make db meter
gcc dB_meter.c -lasound -o dB_meter
