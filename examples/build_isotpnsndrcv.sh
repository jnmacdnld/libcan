mkdir -p bin
gcc isotpnsndrcv.c ../libcan.c -std=gnu99 -I../include -I.. -o bin/isotpnsndrcv