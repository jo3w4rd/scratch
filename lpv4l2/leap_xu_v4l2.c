/*
 *
 * Copyright © 2016 Leap Motion, Inc.
 *
 * V4L2 driver Leap Motion extension unit code
 */
#include "uvcvideo.h"
#include "leap_xu_v4l2.h"

static long handle_xu_operation(void *fh, bool valid_prio, struct uvc_xu_control_query *xqry){
   return 3;
}

long leap_xu_ioctl_default(struct file *file, void *fh, bool valid_prio, unsigned int cmd, void *arg) {
         printk(KERN_ALERT "xu ioctl was indeed called\n");

         switch (cmd) {
        /* Dynamic controls. */
         case UVCIOC_CTRL_QUERY:
                 return handle_xu_operation(fh, valid_prio, arg);

         // Not supported
         case UVCIOC_CTRL_MAP:
                 return -ENOTTY;

         default:
                 return -ENOTTY;
         }
}
