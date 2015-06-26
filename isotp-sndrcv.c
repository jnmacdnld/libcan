#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong number of arguments\n"
               "Usage: isotp-sndrcv <CAN interface> <tx id> <rx id> <byte> "
               "<byte> ...\n");
        exit(1);
    }

    if (strlen(argv[2]) > 3 || strlen(argv[3]) > 3) {
        printf("Only 11-bit arb ids are allowed\n");
        exit(1);
    }

    // Initialize the transmit and receive ids
    int tx_id, rx_id;
    sscanf(argv[3], "%03x", &rx_id);
    sscanf(argv[2], "%03x", &tx_id);

    // Create the ISOTP socket
    struct can_isotp_options opts;
    opts.flags |= CAN_ISOTP_TX_PADDING;
    printf("opts.flags: %03x\n", opts.flags);
    opts.txpad_content = 0;
    int s = can_socket_isotp(argv[1], tx_id, rx_id, &opts);

    // Initialize the message byte array
    int msg_len = argc - 4;
    __u8 msg[msg_len];

    // Fill the message byte array
    for (__u8 i = 0; i < msg_len; i++) {
        int byte;
        sscanf(argv[i + 4], "%02x", &byte);
        msg[i] = (__u8) byte;
    }

    // Do the send-receive operation
    __u8 resp_buf[BUF_SIZE];
    struct timeval timeout = {1, 0};
    int nbytes = can_sndrcv_isotp(s, msg, msg_len, resp_buf, BUF_SIZE,
                                  &timeout);

    // Print the reponse (if any)
    if (nbytes > 0) {
        printf("  %s  %03x   [%d]  ", argv[1], rx_id & 0xfff,
            nbytes);
        for (int k = 0; k < nbytes; k++) {
            printf("%02x ", resp_buf[k]);
        }
        printf("\n");
    }
}
