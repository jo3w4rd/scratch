/*
 *
 * Copyright © 2016 Leap Motion, Inc.
 *
 * V4L2 driver Leap Motion extension unit code
 */
#include <stdlib.h>     /* atol */

#include <linux/uvcvideo.h>
#include "leap_xu_v4l2.h"

static long handle_xu_operation(void *fh, bool valid_prio, struct uvc_xu_control_query *xu_query){
    printk(KERN_ALERT "Query type: %i for selector %i with data %c.\n", xu_query->query, xu_query->selector, (char *)xu_query->data);
    char data = (char *)xu_query->data;
    return atol(data);
}

long leap_xu_ioctl_default(struct file *file, void *fh, bool valid_prio, unsigned int cmd, void *arg) {
         printk(KERN_ALERT "xu ioctl was indeed called\n");
         
         if(cmd == UVCIOC_CTRL_QUERY){
            return handle_xu_operation(fh, valid_prio, arg);
         } else {
            return -ENOTTY;
         }
}
