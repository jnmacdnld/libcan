#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong number of arguments\n"
               "Usage: isotp-sndrcv <CAN interface> <tx id> <rx id> <byte> "
               "<byte> ...\n");
        exit(1);
    }

    // Initialize the transmit and receive ids
    int tx_id, rx_id;
    sscanf(argv[3], "%x", &rx_id);
    sscanf(argv[2], "%x", &tx_id);

    // Start the ISOTP session witht the default options
    struct isotp_sess sess = NEW_DEFAULT_ISOTP_SESS;
    // sess.timeout.tv_sec = 10;
    if (can_start_isotp_sess(&sess, argv[1], tx_id, rx_id) < 0) { return -1; }

    // Initialize the message byte array
    int msg_len = argc - 4;
    __u8 msg[msg_len];

    // Fill the message byte array
    int byte;
    for (__u8 i = 0; i < msg_len; i++) {
        sscanf(argv[i + 4], "%02x", &byte);
        msg[i] = (__u8) byte;
    }

    // Do the send-receive operation
    int nbytes = can_sndrcv_isotp(&sess, msg, msg_len);

    // Print the reponse (if any)
    if (nbytes > 0) {
        printf("Response: ");
        can_print_bytes(sess.buf, nbytes);
    }

    printf("Closing socket...\n");
    can_end_isotp_sess(&sess);
}
