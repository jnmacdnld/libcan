#ifndef SOCKETCAN_H
#define SOCKETCAN_H

#include <linux/can.h>

// Open the specified CAN interface ex: "can0", returns -1 if an error occurs
int can_open(const char *interface);

// Send the specified CAN frame; returns -1 if an error occurs
int can_send(struct can_frame *frame);

// Reads a frame from the CAN bus from the opened interface; returns 1 if a
// frame is read, 0 if the operation times out, and -1 if an error occured
int can_read(struct can_frame *frame);

// Close the CAN interface
int can_close();

#endif
