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
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int s;
    struct ifreq ifr;
    struct sockaddr_can addr;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0");
    ioctl(s, SIOCGIFINDEX, &ifr) < 0;

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0;

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
