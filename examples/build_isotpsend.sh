mkdir -p bin
gcc isotpsend.c ../libcan.c -std=gnu99 -I../include -I.. -o bin/isotpsend