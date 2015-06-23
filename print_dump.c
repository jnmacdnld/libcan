/* Copyright (C) 2015 John Macdonald
 * 
 * This software may not be modified, reused, or redistributed in any way
 * whatsoever as it is the personal property of the individual who wrote it.
 * If you attempt to violate the aformetioned terms, this individual will
 * protect his property though the rights he is granted by the Second Amendment
 * of the United States Consitiution. You have been warned.
 */

#include "socketcan.h"

#include <stdio.h>
#include <linux/can.h>

#define NUM_FRAMES 10

int main(int argc, const char **argv)
{
  can_open("can0");
  struct can_frame frames[NUM_FRAMES];
  can_dump(frames, NUM_FRAMES);

  // Print the frames
  for (int i = 0; i < NUM_FRAMES; i++) {
    printf("[%d] ", frames[i].can_dlc);
    for (int k = 0; k < 8; k++) {
      printf("%02x ", frames[i].data[k]);
    }
    printf("\n");
  }
}