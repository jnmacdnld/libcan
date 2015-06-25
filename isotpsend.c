#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Not enough arguments\n"
               "Usage: cansend <CAN interface> <tx id> <byte> <byte> ...\n");
        exit(0);
    }

    if (strlen(argv[2]) > 3) {
        printf("Only 11-bit arb ids are allowed\n");
        exit(0);
    }

    int tx_id = 0;
    sscanf(argv[2], "%03x", &tx_id);

    // Use 0x000 as a placeholder for the rx id since one isn't needed
    int s = can_socket_isotp(argv[1], tx_id, 0x000);

    int data_len = argc - 3;
    __u8 data[data_len];

    // Fill the data array
    for (__u8 i = 0; i < data_len; i++) {
        int byte;
        sscanf(argv[i + 3], "%02x", &byte);
        data[i] = (__u8) byte;
    }

    can_send_isotp(s, data, data_len);

    printf("Frame sent, closing socket...\n");
    can_close_raw(s);
}
