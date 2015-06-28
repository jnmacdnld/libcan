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
#include <net/if.h>

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

int can_start_isotp_sess(struct isotp_sess *sess, const char *itf, int tx_id,
                         int rx_id)
{
    struct sockaddr_can addr;

    // Store the transmit and receive ids in the socket address
    addr.can_addr.tp.tx_id = (canid_t) tx_id;
    addr.can_addr.tp.rx_id = (canid_t) rx_id;

    sess->s = can_socket_gen(itf, SOCK_DGRAM, CAN_ISOTP, &addr);
    if (sess->s < 0) { return -1; }

    int r = setsockopt(sess->s, SOL_CAN_ISOTP, CAN_ISOTP_OPTS, &(sess->opts),
                       sizeof(sess->opts));
    if (r < 0) { perror("setsockopt in can_start_isotp_sess"); return -1; }

    return 0;
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

int can_send_isotp(struct isotp_sess *sess, __u8 *data, int data_len) {
    int retval = write(sess->s, data, data_len);
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

int can_read_isotp(struct isotp_sess *sess) {
    FD_ZERO( &(sess->rdfs) );
    FD_SET( sess->s, &(sess->rdfs) );

    if (FD_ISSET( sess->s, &(sess->rdfs) )) {
        int nbytes = read(sess->s, sess->buf, ISOTP_BUF_SIZE);
        
        if (nbytes < 0) {
            perror("can_read_isotp read");
            return -1;
        }

        if (nbytes > ISOTP_BUF_SIZE) {
            printf("Received more bytes than fit in the buffer");
            return -1;
        }

        return nbytes;
        
    }    

    return 0;
}

int can_sndrcv_isotp(struct isotp_sess *sess, __u8 *msg, int msg_len) {
    can_send_isotp(sess, msg, msg_len);

    FD_ZERO(&(sess->rdfs));
    FD_SET(sess->s, &(sess->rdfs));

    int nready = select(sess->s + 1, &(sess->rdfs), NULL, NULL,
                        &(sess->timeout));
    if (nready > 0 && FD_ISSET((sess->s), &(sess->rdfs))) {
        int nbytes = read(sess->s, sess->buf, ISOTP_BUF_SIZE);
        
        if (nbytes < 0) {
            perror("read in can_sndrcv_isotp");
            return -1;
        }

        if (nbytes > ISOTP_BUF_SIZE) {
            printf("Received more bytes than fit in the buffer");
            return -1;
        }

        return nbytes;
        
    } else if (nready == 0) {
        printf("can_sndrecv_isotp: No response\n");
        return 0;
    } else {
        perror("select in can_sndrcv_isotp");
        return -1;
    }

    return -1;
}

int can_close_raw(int s) {
    if (close(s) < 0) {
        perror("can_close_raw close");
        return -1;
    } else {
        return 0;
    }
}

int can_end_isotp_sess(struct isotp_sess *sess) {
    can_close_raw(sess->s);
}

void can_print_bytes(__u8 *buf, int nbytes) {
    for (int k = 0; k < nbytes; k++) {
        printf("%02x ", buf[k]);
    }
    printf("\n");
}