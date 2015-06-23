// Large portions of code re-used from
// https://lnguin.wordpress.com/tag/socketcan-example/

#include "socketcan.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <errno.h>

static int soc;
static int read_can_port;

int can_open(const char *interface)
{
  struct ifreq ifr;
  struct sockaddr_can addr;

  // Open socket
  soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if(soc < 0)
  {
    printf("Couldn't open port: socket(2) encountered error %d, exiting",
      errno);
    return -1;
  }

  addr.can_family = AF_CAN;
  strcpy(ifr.ifr_name, interface);

  if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0)
  {
    printf("Couldn't open port: ioctl(2) encountered error %d\n", errno);
    return -1;
  }

  addr.can_ifindex = ifr.ifr_ifindex;

  fcntl(soc, F_SETFL, O_NONBLOCK);

  if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    printf("Couldn't open port: bind(2) encountered error %d\n", errno);
    return -1;
  }

  return 0;
}

int can_send(struct can_frame *frame)
{
  int retval;
    retval = write(soc, frame, sizeof(struct can_frame));
  if (retval != sizeof(struct can_frame))
  {
    printf("Couldn't send frame: write(2) encountered error %d\n", errno);
    return -1;
  }
  else
  {
    return 0;
  }
}

int can_dump(struct can_frame *dump, int num_frames)
{
  struct can_frame frame_rd;
  int recvbytes = 0;
  
  for (int i = 0; i < num_frames; i++)
  {
    struct timeval timeout = {1, 0};
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(soc, &readSet);
    if (select((soc + 1), &readSet, NULL, NULL, &timeout) >= 0)
    {
      if (FD_ISSET(soc, &readSet))
      {
        recvbytes = read(soc, &frame_rd, sizeof(struct can_frame));
        if(recvbytes)
        {
          dump[i] = frame_rd;
        }
      }
    }
    else
    {
      printf("Couldn't dump CAN traffic: select(2) encountered error %d\n",
        errno);
      return -1;
    }
  }
}

int can_close()
{
  close(soc);
  return 0;
}
