/*  ioctl.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process. 
 */

/* device specifics, such as ioctl numbers and the 
 * major device file. */
#include "chardev.h"    


#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* Functions for the ioctl calls */

int ioctl_set_msg(int file_desc, char *message)
{
  int ret_val;

  ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

  if (ret_val < 0) {
    printf ("ioctl_set_msg failed:%d\n", ret_val);
  }
  
  return ret_val;
}



int ioctl_get_msg(int file_desc)
{
  int ret_val;
  char message[100]; 

  /* Warning - this is dangerous because we don't tell 
   * the kernel how far it's allowed to write, so it 
   * might overflow the buffer. In a real production 
   * program, we would have used two ioctls - one to tell
   * the kernel the buffer length and another to give 
   * it the buffer to fill
   */
  ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);

  if (ret_val < 0) {
    printf ("ioctl_get_msg failed:%d\n", ret_val);
  }

  printf("get_msg message:%s\n", message);
  return ret_val;
}



void ioctl_get_nth_byte(int file_desc)
{
  int i;
  char c;

  printf("get_nth_byte message:");

  i = 0;
  while (c != 0) {
    c = ioctl(file_desc, IOCTL_GET_NTH_BYTE, i++);

    if (c < 0) {
      printf(
      "ioctl_get_nth_byte failed at the %d'th byte:\n", i);
      exit(-1);
    }

    putchar(c);
  } 
  putchar('\n');
}




/* Main - Call the ioctl functions */
int main()
{
  int file_desc, ret_val;
  char *msg = "Message passed by ioctl\n";

  file_desc = open(DEVICE_FILE_NAME, O_RDWR);
  int errsv = errno;
  if (file_desc < 0) {
    printf ("Can't open device file: %s, because %s\n", 
            DEVICE_FILE_NAME, strerror(errsv));
    exit(-1);
  }

  ret_val = ioctl_get_msg(file_desc);
  ret_val = ioctl_set_msg(file_desc, msg);

  close(file_desc); 
  
  return ret_val;
}