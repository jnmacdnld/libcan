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
#include <linux/can/isotp.h>

static int can_socket_gen(const char *itf, int type, int can_protocol,
                          struct sockaddr_can *addr)
{
    int s;
    struct ifreq ifr;

    s = socket(PF_CAN, type, can_protocol);
    if (s < 0) {
        perror("can_socket_gen socket");
        return -1;
    }

    strcpy(ifr.ifr_name, itf);

    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        perror("can_socket_gen ioctl");
        return -1;
    }

    addr->can_family = AF_CAN;
    addr->can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *) addr, sizeof(*addr)) < 0) {
        perror("can_socket_gen bind");
        return -1;
    }

    return s;
}

int can_socket_raw(const char *itf) {
    struct sockaddr_can addr;
    return can_socket_gen(itf, SOCK_RAW, CAN_RAW, &addr);
}

int can_socket_isotp(const char *itf, int tx_id, int rx_id,
                     struct can_isotp_options *opts)
{
    struct sockaddr_can addr;

    // Store the tx and rx ids in the socket address
    addr.can_addr.tp.tx_id = (canid_t) tx_id;
    addr.can_addr.tp.rx_id = (canid_t) rx_id;

    int s = can_socket_gen(itf, SOCK_DGRAM, CAN_ISOTP, &addr);

    setsockopt(s, SOL_CAN_ISOTP, CAN_ISOTP_OPTS, &opts, sizeof(opts));

    return s;
}

int can_send_raw(int s, struct can_frame *frame) {
    int retval = write(s, frame, sizeof(struct can_frame));

    if (retval != sizeof(struct can_frame)) {
        perror("can_send_raw write");
        return -1;
    } else {
        return 0;
    }
}

int can_send_isotp(int s, __u8 *data, int data_len) {
    int retval = write(s, data, data_len);
    if (retval < 0) {
        perror("can_send_isotp write");
        return -1;
    } else {
        return 0;
    }
}

int can_read_raw(int s, struct can_frame *frame) {
    int nbytes = read(s, frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("can_read_raw read");
        return -1;
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Incomplete CAN frame\n");
        return -1;
    }

    return 0;
}

int can_read_isotp(int s, __u8 *buf, int buf_size) {
    fd_set rdfs;

    FD_ZERO(&rdfs);
    FD_SET(s, &rdfs);

    if (FD_ISSET(s, &rdfs)) {
        int nbytes = read(s, buf, buf_size);
        
        if (nbytes < 0) {
            perror("can_read_isotp read");
            return -1;
        }

        if (nbytes > buf_size) {
            printf("Received more bytes than fit in the buffer");
            return -1;
        }

        return nbytes;
        
    }    

    return 0;
}

int can_close_raw(int s) {
    if (close(s) < 0) {
        perror("can_close_raw close");
        return -1;
    } else {
        return 0;
    }
}

int can_close_isotp(int s) {
    can_close_raw(s);
}

    // // Make a timeval struct that represents one second
    // struct timeval timeout = {1, 0};

    // fd_set read_set;
    // FD_ZERO(&read_set);
    // FD_SET(s, &read_set);

    // if (select(s + 1, &read_set, NULL, NULL, &timeout) >= 0
    //     && FD_ISSET(s, &readSet))
    // {
    //     recvbytes = read(s, data, 8);
    //     if(recvbytes)
    //     {
    //         printf(“dlc = %d, data = %s\n”, frame_rd.can_dlc,frame_rd.data);
    //     }
    // }