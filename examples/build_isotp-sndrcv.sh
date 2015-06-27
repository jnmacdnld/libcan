mkdir -p bin
gcc isotp-sndrcv.c ../libcan.c -std=gnu99 -I../include -I.. -o bin/isotp-sndrcv