#include "libcan.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Wrong number of arguments\n"
               "Usage: isotpdump <CAN interface> <rx id>\n");
        exit(1);
    }

    if (strlen(argv[2]) > 3) {
        printf("Only 11-bit arb ids are allowed\n");
        exit(1);
    }

    int rx_id = 0;
    sscanf(argv[2], "%03x", &rx_id);

    struct can_isotp_options opts;
    int s = can_socket_isotp(argv[1], 0x000, rx_id, &opts);

    __u8 data[BUF_SIZE];

    fd_set rdfs;

    while (1) {
        int nbytes = can_read_isotp(s, data, BUF_SIZE, &rdfs);
        if (nbytes > 0) {
            printf("  %s  %03x   [%d]  ", argv[1], rx_id & 0xfff,
                nbytes);
            for (int k = 0; k < nbytes; k++) {
                printf("%02x ", data[k]);
            }
            printf("\n");
        }
    }
}
