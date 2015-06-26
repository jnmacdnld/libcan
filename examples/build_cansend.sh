mkdir -p bin
gcc cansend.c ../libcan.c -std=gnu99 -I../include -I.. -o bin/cansend
