#!/bin/sh

#sudo apt-get install libsndfile1-dev 
mkdir -p bin
gcc  -I/usr/local/include -O2 -c testsnd.c 
libtool  --mode=link gcc  -g -O2 -std=gnu99 -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Waggregate-return -Wcast-align -Wcast-qual -Wnested-externs -Wshadow -Wpointer-arith -o testsnd ./testsnd.o -L/usr/local/lib -lsndfile   -lpthread -lrt   -lm 
mv testsnd bin
mv testsnd.o snd

#make db meter
gcc dB_meter.c -lasound -o bin/dB_meter -lm
gcc dB_meter_bar.c -lasound -o bin/dB_meter_bar -lm


#make simplerecords
gcc -o bin/simpleloop  simpleloop.c -lasound -lm
gcc -o bin/simpleplay  simpleplay.c -lasound -lm
gcc -o bin/simplerecord  simplerecord.c -lasound -lm
