#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Not enough arguments\n"
           "Usage: cansend <CAN interface> <arb id> <byte> <byte> ...");
    exit(0);
  }

  
}