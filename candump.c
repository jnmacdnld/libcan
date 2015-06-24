/* Copyright (C) 2015 John Macdonald
 * 
 * This software may not be modified, reused, or redistributed in any way
 * whatsoever as it is the personal property of the individual who wrote it.
 * If you attempt to violate the aformetioned terms, this individual will
 * protect his property though the rights he is granted by the Second Amendment
 * of the United States Consitiution. You have been warned.
 */

#include "libcan.h"

#include <stdio.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong number of arguments\nUsage: candump <CAN interface>");
        exit(1);
    }

    int s = can_open(argv[1]);

    while (1) {
        struct can_frame frame;
        
        int nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes > 0) {
            printf("  %s  %03x   [%d]  ", "can0", frame.can_id & 0xfff,
                frame.can_dlc);
            for (int k = 0; k < frame.can_dlc; k++) {
                printf("%02x ", frame.data[k]);
            }
            printf("\n");
        }
    }
}
