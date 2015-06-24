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

static int can_socket_gen(const char *itf, int type, int can_protocol,
                      struct sockaddr_can *addr)
{
    int s;
    struct ifreq ifr;

    s = socket(PF_CAN, type, can_protocol);
    if (s < 0) {
        perror("Couldn't open CAN socket");
        return -1;
    }

    strcpy(ifr.ifr_name, itf);

    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        perror("Couldn't open CAN socket");
        return -1;
    }

    addr->can_family = AF_CAN;
    addr->can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *) addr, sizeof(*addr)) < 0) {
        perror("Couldn't open CAN socket");
        return -1;
    }

    return s;
}

int can_socket_raw(const char *itf) {
    struct sockaddr_can addr;
    return can_socket_gen(itf, SOCK_RAW, CAN_RAW, &addr);
}

int can_socket_isotp(const char *itf, int tx_id, int rx_id) {
    struct sockaddr_can addr;

    // Store the tx and rx ids in the socket address
    addr.can_addr.tp.rx_id = (canid_t) tx_id;
    addr.can_addr.tp.tx_id = (canid_t) rx_id;

    return can_socket_gen(itf, SOCK_DGRAM, CAN_ISOTP, &addr);
}

int can_send_raw(int s, struct can_frame *frame) {
    int retval = write(s, frame, sizeof(struct can_frame));

    if (retval != sizeof(struct can_frame)) {
        perror("Couldn't send frame");
        return -1;
    } else {
        return 0;
    }
}

int can_read_raw(int s, struct can_frame *frame) {
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

int can_close_raw(int s) {
    close(s);
    return 0;
}