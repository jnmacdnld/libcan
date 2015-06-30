mkdir -p bin
gcc isotpsndrcv.c ../libcan.c -std=gnu99 -I../include -I.. -o bin/isotpsndrcv