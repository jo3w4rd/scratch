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
#include <stdint.h>
#include <linux/usb/video.h>
#include <linux/uvcvideo.h>

#include "leap_xu.h"

/* Functions for the ioctl calls */

// uvc_xu_control_query ::
// __u8 unit;
// __u8 selector;
// __u8 query;        /* Video Class-Specific Request Code, */
//             /* defined in linux/usb/video.h A.8.  */
// __u16 size;
// __u8 *data;
int query_set_strobe(int file_desc){
    int ret_val;
    uint32_t dataChar = 0xffffffff;
    
    struct uvc_xu_control_query args = {};
    printf("Init struct\n");    
    args.unit = 12;
    args.selector = LEAP_XU_STROBE_WIDTH;
    args.query = UVC_SET_CUR;
    args.size = sizeof(uint32_t);
    args.data = (__u8 *)&dataChar;

    printf("Calling set cur\n");    
    ret_val = ioctl(file_desc, UVCIOC_CTRL_QUERY, &args);
    printf("Returned from set cur with value %lu\n", (long unsigned)dataChar);

    return ret_val;
}

int query_get_devcaps(int file_desc){
    int ret_val;
    LEAP_DEVCAPS caps = {};
    
    struct uvc_xu_control_query args = {};
    printf("Init struct\n");    
    args.unit = 12;
    args.selector = LEAP_XU_DEVCAPS;
    args.query = UVC_GET_CUR;
    args.size = sizeof(uint32_t);
    args.data = (__u8 *)&caps;

    printf("Calling get cur devcaps\n");    
    ret_val = ioctl(file_desc, UVCIOC_CTRL_QUERY, &args);
    printf("Devcaps: 0x%08X, %lu, %u, %u, %s\n", caps.flags, (long unsigned int)caps.firmware_rev, caps.controller_id, caps.sensor_id,caps.serial);

    return ret_val;
}

int query_get_strobe(int file_desc){
    int ret_val;
    uint32_t dataChar = 5;
    
    struct uvc_xu_control_query args = {};
    printf("Init struct\n");    
    args.unit = 12;
    args.selector = LEAP_XU_STROBE_WIDTH;
    args.query = UVC_GET_CUR;
    args.size = sizeof(uint32_t);
    args.data = (__u8 *)&dataChar;

    printf("Calling get cur\n");    
    ret_val = ioctl(file_desc, UVCIOC_CTRL_QUERY, &args);
    printf("Returned from get cur with value %lu\n", (long unsigned)dataChar);

    return ret_val;
}

int query_info(int file_desc){
    int ret_val;
    uint8_t dataChar = 3;
    
    struct uvc_xu_control_query args = {};
    printf("Init struct\n");    
    args.unit = 12;
    args.selector = LEAP_XU_DEVCAPS;
    args.query = UVC_GET_INFO;
    args.size = 1;
    args.data = (__u8 *)&dataChar;

    printf("Calling get info\n");    
    ret_val = ioctl(file_desc, UVCIOC_CTRL_QUERY, &args);
    printf("Returned from get info\n");
    uint8_t cgs = (uint8_t)*args.data;
    printf("access data\n");
    char *canGet = cgs & 0x01 ? "yes" : "no";
    char *canSet = cgs & 0x02 ? "yes" : "no";
    printf("make char strings\n");
    printf("Get: %s, Set %s\n", canGet, canSet);

    return ret_val;
}
int query_length(int file_desc){
    int ret_val;
    uint16_t dataChar = 22;
    
    struct uvc_xu_control_query args = {};
    args.unit = 12;
    args.selector = LEAP_XU_DEVCAPS;
    args.query = UVC_GET_LEN;
    args.size = 2;
    args.data = (__u8 *)&dataChar;
    
    ret_val = ioctl(file_desc, UVCIOC_CTRL_QUERY, &args);
    printf("Query returned: %i, and data %lu\n", ret_val, (unsigned long)*args.data);

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

  ret_val = query_length(file_desc);
  if(ret_val < 0){
      printf ("Query returned error: %i, %s\n", ret_val, strerror(errsv));
  }
  ret_val = query_info(file_desc);
  if(ret_val < 0){
      printf ("Query returned error: %i, %s\n", ret_val, strerror(errsv));
  }
  
  ret_val = query_get_strobe(file_desc);
  if(ret_val < 0){
      printf ("Query returned error: %i, %s\n", ret_val, strerror(errsv));
  }

  ret_val = query_set_strobe(file_desc);
  if(ret_val < 0){
      printf ("Query returned error: %i, %s\n", ret_val, strerror(errsv));
  }

  ret_val = query_get_devcaps(file_desc);
  if(ret_val < 0){
      printf ("Query returned error: %i, %s\n", ret_val, strerror(errsv));
  }

  close(file_desc); 
  
  return ret_val;
}