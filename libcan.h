#ifndef LIBCAN_H
#define LIBCAN_H

#include <linux/can.h>
#include <linux/can/isotp.h>
#include <time.h>
#include <net/if.h>

#define NEG_RESP_SID 0x7F
#define POS_RESPONSE 0x00
#define SUB_FUNC_NOT_SUPPORTED 0x12
#define SECURITY_ACCESS_DENIED 0x33
#define REQ_OUT_OF_RANGE 0x31

#define DIAG_SESS_CTRL_REQ_SID 0x10
#define DIAG_SESS_CTRL_RESP_SID 0x50
#define DEFAULT_SESS 0x01
#define EXTENDED_DIAG_SESS 0x02

#define RTNE_CTRL_REQ_SID 0x31
#define RTNE_CTRL_RESP_SID 0x71
#define START_RTNE 0x01
#define STOP_RTNE 0x02

#define RESP_PENDING 0x78

#define ARB_ID(frame) frame.can_id & 0xfff

#define ISOTP_BUF_SIZE 10000
#define NO_CAN_ID 0xFFFF

#define NEW_DEFAULT_ISOTP_SESS {                                              \
                                   .opts = {.flags = CAN_ISOTP_TX_PADDING},   \
                                   .timeout = {1, 0}                          \
                               }

struct isotp_sess {
    int s;
    struct can_isotp_options opts;
    struct timespec timeout;
    fd_set rdfs;
    __u8 buf[ISOTP_BUF_SIZE];
};

// All functions return 0 on success and -1 on failure unless otherwise noted

// Creates an raw CAN socket and returns it
int can_socket_raw(const char *itf);

// Start an ISOTP session
int can_start_isotp_sess(struct isotp_sess *sess, const char *itf, int tx_id,
                         int rx_id);

// Sends a CAN frame
int can_send_raw(int soc, struct can_frame *frame);

// Sends a CAN frame using the ISOTP protocol, does not support data lengths
// greater than 7
int can_send_isotp(struct isotp_sess *sess, __u8 *data, int data_len);

// Reads a frame from the CAN bus from the opened interface, blocks until a
// message is received
int can_read_raw(int soc, struct can_frame *frame);

// Reads a message from the CAN bus from the opened interface; returns number of 
// bytes recived on success, blocks until a message is received
int can_read_isotp(struct isotp_sess *sess);

// Send the supplied message and write the response into the supplied buffer,
// returns the number of bytes in the response on success
int can_sndrcv_isotp(struct isotp_sess *sess, __u8 *msg, int msg_len);

// Close the specified raw CAN socket
int can_close_raw(int soc);

// Tnd the specified ISOTP session
int can_end_isotp_sess(struct isotp_sess *sess);

// Prints the bytes from the buffer with a newline at the end
void can_print_bytes(__u8 *buf, int buf_len);

#endif /* LIBCAN_H */
