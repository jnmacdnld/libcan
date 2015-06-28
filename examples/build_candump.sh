mkdir -p bin
gcc candump.c ../libcan.c -std=gnu99 -I../include -I.. -o bin/candump