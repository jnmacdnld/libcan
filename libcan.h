#ifndef LIBCAN_H
#define LIBCAN_H

#include <linux/can.h>
#include <linux/can/isotp.h>
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

struct isotp_sess {
	int s;
	__u8 buf[ISOTP_BUF_SIZE];
	fd_set rdfs;
	struct sockaddr_can addr;
	can_isotp_options opts;
};

// Creates an raw CAN socket and returns it or -1 if an error occurs
int can_socket_raw(const char *itf);

// Creates an ISOTP CAN socket and returns it or -1 if an error occurs
int can_socket_isotp(const char *itf, int tx_id, int rx_id,
                     struct can_isotp_options *opts);

// Creates an ISOTP CAN socket with options set to pad the transmitted frames
// with zeroes
int can_socket_isotp_txpad(const char *itf, int tx_id, int rx_id,
                           struct can_isotp_options *opts);


int start_isotp_sess(const char *itf, int tx_id, int rx_id,
                     struct isotp_sess *sess);

// Sends a CAN frame; returns 0 on success and -1 on failure
int can_send_raw(int s, struct can_frame *frame);

// Sends a CAN frame using the ISOTP protocol; returns 0 on success and -1 on
// failure. Does not support data lengths greater than 7
int can_send_isotp(int s, __u8 *data, int data_len);

// Reads a frame from the CAN bus from the opened interface; returns 0 on
// success and -1 on failure
// Blocks until a message is received
int can_read_raw(int s, struct can_frame *frame);

// Reads a message from the CAN bus from the opened interface; returns number of 
// bytes recived on success and -1 on failure
// Blocks until a message is received
int can_read_isotp(int s, __u8 *buf, int buf_size, fd_set *rdfs);

// Send the supplied message and write the response into the supplied buffer;
// returns the number of bytes in the response on success and -1 on failure
int can_sndrcv_isotp(int s, __u8 *msg, int msg_len, __u8 *resp_buf,
                     int resp_buf_len, struct timeval *timeout);

// Close the specified raw CAN socket
int can_close_raw(int s);

// Close the specified ISOTP CAN socket
int can_close_isotp(int s);

#endif /* LIBCAN_H */
