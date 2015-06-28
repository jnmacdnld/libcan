#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

int main(int argc, char *argv[]) {
    // Ensure enough arguments are provided by the user
    if (argc < 3) {
        printf("Wrong number of arguments\n"
               "Usage: isotpdump <CAN interface> <rx id>\n");
        exit(1);
    }

    int nbytes, rx_id = 0;
    struct isotp_sess sess = {0};

    // Copy the receive id the user provided into rx_id
    sscanf(argv[2], "%x", &rx_id);

    // Start the ISOTP session
    if (start_isotp_sess(argv[1], NO_CAN_ID, rx_id, &sess) < 0) {
        exit(1);
    }

    // Print the received messages
    while (1) {
        nbytes = can_read_isotp(&sess);
        if (nbytes > 0) {
            printf("  %s  %03x   [%d]  ", argv[1], rx_id & 0xfff, nbytes);
            print_bytes(sess.buf, nbytes);
        }
    }
}
