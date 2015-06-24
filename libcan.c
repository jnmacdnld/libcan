#include "libcan.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <errno.h>

int can_open(const char *itf) {
    int s;
    struct ifreq ifr;
    struct sockaddr_can addr;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(s < 0) {
        perror("Couldn't open port");
        return -1;
    }

    strcpy(ifr.ifr_name, itf);

    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        perror("Couldn't open port");
        return -1;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // fcntl(s, F_SETFL, O_NONBLOCK);

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Couldn't open port");
        return -1;
    }

    return s;
}

int can_read(int s, struct can_frame *frame) {
    int nbytes = read(s, frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("Couldn't send frame");
        return -1;
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Incomplete CAN frame\n");
        return -1;
    }

    return 0;
}

int can_close(int s) {
    close(s);
    return 0;
}

// Large portions of code below re-used from
// https://lnguin.wordpress.com/tag/socketcan-example/

// int can_read(struct can_frame *frame) {
//     struct can_frame frame_rd;
//     int recvbytes = 0;
    
//     struct timeval timeout = {1, 0};
//     fd_set readSet;
//     FD_ZERO(&readSet);
//     FD_SET(s, &readSet);
//     if (select((s + 1), &readSet, NULL, NULL, &timeout) >= 0) {
//         if (FD_ISSET(s, &readSet)) {
//             recvbytes = read(s, &frame_rd, sizeof(struct can_frame));
//             if (recvbytes) {
//                 *frame = frame_rd;
//                 return 1;
//             } else {
//                 return 0;
//             }
//         }
//     } else {
//         printf("Couldn't read frame: select(2) encountered error %d\n",
//             errno);
//         return -1;
//     }
// }
