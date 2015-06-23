#ifndef SOCKETCAN_H
#define SOCKETCAN_H

#include <linux/can.h>

// Open the specified CAN interface ex: "can0"
int can_open(const char *interface);

// Send the specified CAN frame 
int can_send(struct can_frame *frame);

// Dump all of the frames on the CAN bus into the specified array up to the
// specified number of frames
int can_dump(struct can_frame *dump, int num_frames);

// Close the CAN interface
int can_close();

#endif
