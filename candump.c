#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong number of arguments\nUsage: candump <CAN interface>");
        exit(1);
    }

    int s = can_open_raw(argv[1]);

    while (1) {
        struct can_frame frame;

        if (can_read_raw(s, &frame) == 0) {
            printf("  %s  %03x   [%d]  ", "can0", ARB_ID(frame),
                frame.can_dlc);
            for (int k = 0; k < frame.can_dlc; k++) {
                printf("%02x ", frame.data[k]);
            }
            printf("\n");
        }
    }
}
