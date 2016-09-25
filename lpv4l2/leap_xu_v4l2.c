/*
 *
 * Copyright © 2016 Leap Motion, Inc.
 *
 * V4L2 driver Leap Motion extension unit code
 */
#include <linux/types.h>
#include <linux/uvcvideo.h>
#include "leap_xu_v4l2.h"

struct leap_xu_ctrl;

typedef int(leap_xu_ctrl_cb)(
            void *fh,
            const struct leap_xu_ctrl *xu_ctrl,
            void *data
            );

struct leap_xu_ctrl{
    __u8 id;
    __u16 dataSize;
    leap_xu_ctrl_cb *setter;
    leap_xu_ctrl_cb *getter;
};


static const struct leap_xu_ctrl leap_xu_ctrls[] = {};
//    {LEAP_XU.LEAP_XU_STROBE_WIDTH, sizeof(u32_t), NULL, NULL},
//    {LEAP_XU.LEAP_XU_LED_POSITIONS, sizeof(u8_t), NULL, NULL}
//};
static const int num_leap_xu_ctrls = sizeof( leap_xu_ctrls ) / sizeof( leap_xu_ctrls[0] );

static int find_leap_xu_ctrl(__u8 selector, leap_xu_ctrl *ctrl){
    for(int c = 0; c < num_leap_xu_ctrls; c++){
        if(leap_xu_ctrls[c].selector == selector){
            ctrl = leap_xu_ctrls[c];
            return 0;
        }
    }
    return -EINVAL;
}

static long handle_xu_operation(void *fh, bool valid_prio, struct uvc_xu_control_query *xu_query){
    leap_xu_ctrl *xu_ctrl;
    if(find_leap_xu_ctrl(xu_query->selector, xu_ctrl)){
        return -EINVAL;
    } else {
        switch(xu_query->query){
            case UVC_SET_CUR:
                if(xu_ctrl->setter){
                    return xu_ctrl->setter(fh, xu_ctrl, data);
                } else {
                    return -EBADRQC;
                }
            case UVC_GET_CUR:
                if(xu_ctrl->getter){
                    return xu_ctrl->getter(fh, xu_ctrl, data);
                } else {
                    return -EBADRQC;
                }
            case UVC_GET_LEN:
                xu_query->data = xu_ctrl->dataSize;
                return 0;
            case UVC_GET_INFO:
                xu_query->data = 0;
                if(xu_ctrl->getter) xu_query->data |= 0x01;
                if(xu_ctrl->setter) xu_query->data |= 0x02;
                return 0;
            default:
                return -EINVAL;
        }
    }
}

long leap_xu_ioctl_default(struct file *file, void *fh, bool valid_prio, unsigned int cmd, void *arg) {
         printk(KERN_ALERT "xu ioctl was indeed called\n");
         
         if(cmd == UVCIOC_CTRL_QUERY){
            return handle_xu_operation(fh, valid_prio, arg);
         } else {
            return -ENOTTY;
         }
}
