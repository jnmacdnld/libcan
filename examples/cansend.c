#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Not enough arguments\n"
               "Usage: cansend <CAN interface> <arb id> <byte> <byte> ...\n");
        exit(0);
    }

    if (strlen(argv[2]) > 3) {
        printf("Only 11-bit arb ids are allowed\n");
        exit(0);
    }

    int s = can_socket_raw(argv[1]);
    struct can_frame frame;
    int arb_id = 0;

    sscanf(argv[2], "%03x", &arb_id);

    __u8 data_len = (__u8) (argc - 3);

    frame.can_id = (canid_t) arb_id;
    frame.can_dlc = (__u8) data_len;

    // Fill the array of data bytes
    for (__u8 i = 0; i < data_len; i++) {
        int byte;
        sscanf(argv[i + 3], "%02x", &byte);
        frame.data[i] = (__u8) byte;
    }

    can_send_raw(s, &frame);
    can_close_raw(s);
}
