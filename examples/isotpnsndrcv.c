#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong number of arguments\n"
               "Usage: isotp-sndrcv <CAN interface> <tx id> <rx id> <byte> "
               "<byte> ...\n");
        exit(1);
    }

    // Initialize the transmit and receive ids
    int tx_id, rx_id, n_ops;
    sscanf(argv[2], "%x", &tx_id);
    sscanf(argv[3], "%x", &rx_id);
    sscanf(argv[4], "%d", &n_ops);

    // Start the ISOTP session witht the default options
    struct isotp_sess sess = NEW_DEFAULT_ISOTP_SESS;
    // sess.timeout.tv_sec = 10;
    if (can_start_isotp_sess(&sess, argv[1], tx_id, rx_id) < 0) { return -1; }

    // Initialize the message byte array
    int msg_len = argc - 5;
    __u8 msg[msg_len];

    // Fill the message byte array
    int byte;
    for (__u8 i = 0; i < msg_len; i++) {
        sscanf(argv[i + 5], "%02x", &byte);
        msg[i] = (__u8) byte;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int nbytes;

    for (int i = 0; i < n_ops; i++) {
        // sess.timeout.tv_sec = 1;
        // sess.timeout.tv_usec = 0;
        nbytes = can_sndrcv_isotp(&sess, msg, msg_len);
    }

    gettimeofday(&end, NULL);
    int diff_usec = (end.tv_usec + (end.tv_sec * 1000000))
                    - (start.tv_usec + (start.tv_sec * 1000000));
    float diff_msec = diff_usec / 1000.0;

    printf("Done (%f msec total, %f per op)\n", diff_msec,
        diff_msec / n_ops);
    
    

    printf("Closing socket...\n");
    can_end_isotp_sess(&sess);
}
