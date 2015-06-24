#ifndef LIBCAN_H
#define LIBCAN_H

#include <linux/can.h>

// Open the specified CAN interface and returns a socket for it ex: "can0",
// returns -1 if an error occurs
int can_socket(const char *interface);

// Send the specified CAN frame; returns -1 if an error occurs
int can_send(struct can_frame *frame);

// Reads a frame from the CAN bus from the opened interface; returns 1 if a
// frame is read, and -1 if an error occured
// Buggy, use 
int can_read(struct can_frame *frame);

// Close the CAN interface
int can_close();

#endif /* LIBCAN_H */
