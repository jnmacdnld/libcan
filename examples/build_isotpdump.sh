mkdir -p bin
gcc isotpdump.c ../libcan.c -std=gnu99 -I../include -I.. -o bin/isotpdump