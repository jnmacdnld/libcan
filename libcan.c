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

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Couldn't open port");
        return -1;
    }

    return s;
}

int can_send(int s, struct can_frame *frame) {
    int retval;
        retval = write(s, frame, sizeof(struct can_frame));
    if (retval != sizeof(struct can_frame)) {
        perror("Couldn't send frame");
        return -1;
    } else {
        return 0;
    }
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