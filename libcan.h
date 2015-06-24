#ifndef LIBCAN_H
#define LIBCAN_H

#include <linux/can.h>

#define ARB_ID(frame) frame.can_id & 0xfff

// Open the specified CAN interface and returns a socket for it ex: "can0",
// returns -1 if an error occurs
int can_socket_raw(const char *itf);

// Sends a frame with the specified arbitration id and data, returns 0 on
// success and -1 on failure
int can_send_raw(int s, struct can_frame *frame);

// Reads a frame from the CAN bus from the opened interface; returns 0 on
// success and -1 on failure
int can_read_raw(int s, struct can_frame *frame);

// Close the CAN interface
int can_close_raw(int s);

int can_socket_isotp(const char *itf, int tx_id, int rx_id);

// can_send_isotp(struct isotp_socket, int data_len, __u8 *data);



#endif /* LIBCAN_H */
