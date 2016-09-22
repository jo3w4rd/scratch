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

#include <linux/usb/video.h>

#include "uvcvideo.h"
#include "leap_xu.h"

/* Functions for the ioctl calls */


// __u8 unit;
// __u8 selector;
// __u8 query;        /* Video Class-Specific Request Code, */
//             /* defined in linux/usb/video.h A.8.  */
// __u16 size;
// __u8 *data;

int query_info(int file_desc){
    int ret_val;
    
    struct uvc_xu_control_query args;
    args.unit = 12;
    args.selector = LEAP_XU_STROBE_WIDTH;
    args.query = UVC_GET_INFO;
    args.size = 1;
    args.data = malloc(args.size);
    
    ret_val = ioctl(file_desc, UVCIOC_CTRL_QUERY, &args);
    printf("Query returned: %i\n", ret_val);

    free(args.data);
    return ret_val;
}


/* Main - Call the ioctl functions */
int main()
{
  int file_desc, ret_val;

  file_desc = open(DEVICE_FILE_NAME, O_RDWR);
  int errsv = errno;
  if (file_desc < 0) {
    printf ("Can't open device file: %s, because %s\n", 
            DEVICE_FILE_NAME, strerror(errsv));
    exit(-1);
  }

  ret_val = query_info(file_desc);
  printf("Query returned: %i\n", ret_val);

  close(file_desc); 
  
  return ret_val;
}