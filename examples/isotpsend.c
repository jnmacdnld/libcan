#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/can/isotp.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Not enough arguments\n"
               "Usage: isotpsend <CAN interface> <tx id> <byte> <byte> ...\n");
        exit(1);
    }

    int tx_id = 0;
    struct isotp_sess sess = {0};
    int data_len = argc - 3;
    __u8 data[data_len];

    // Copy the user provided transmit id into tx_id
    sscanf(argv[2], "%x", &tx_id);

    // Start the ISOTP session
    if (start_isotp_sess(&sess, argv[1], tx_id, NO_CAN_ID) < 0) {
        exit(-1);
    }

    // Fill the data array
    int byte;
    for (int i = 0; i < data_len; i++) {
        sscanf(argv[i + 3], "%02x", &byte);
        data[i] = (__u8) byte;
    }

    can_send_isotp(&sess, data, data_len);

    printf("Frame sent, closing socket...\n");
    end_isotp_sess(&sess);
}
