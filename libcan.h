#ifndef LIBCAN_H
#define LIBCAN_H

#include <linux/can.h>

#define ARB_ID(frame) frame.can_id & 0xfff

// Creates an raw CAN socket and returns it or -1 if an error occurs
int can_socket_raw(const char *itf);

// Creates an ISOTP CAN socket and returns it or -1 if an error occurs
int can_socket_isotp(const char *itf, int tx_id, int rx_id);

// Sends a CAN frame; returns 0 on success and -1 on failure
int can_send_raw(int s, struct can_frame *frame);

// Sends a CAN frame using the ISOTP protocol; returns 0 on success and -1 on
// failure. Does not support data lengths greater than 7
int can_send_isotp(int s, __u8 *data, int data_len);

// Reads a frame from the CAN bus from the opened interface; returns 0 on
// success and -1 on failure
int can_read_raw(int s, struct can_frame *frame);

// Close the specified raw CAN socket
int can_close_raw(int s);

// Close the specified ISOTP CAN socket
int can_close_isotp(int s);

#endif /* LIBCAN_H */
