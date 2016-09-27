/*
 *
 * Copyright © 2016 Leap Motion, Inc.
 *
 * V4L2 driver Leap Motion extension unit code
 */
#include <linux/types.h>
#include <linux/usb/video.h>
#include <linux/uvcvideo.h>
#include "leap_xu_v4l2.h"
#include "leap_xu.h"

struct leap_xu_ctrl;

typedef int(leap_xu_ctrl_cb)(
            void *fh,
            const struct leap_xu_ctrl *xu_ctrl,
            void *data
            );

struct leap_xu_ctrl{
    __u8 selector;
    __u16 dataSize;
    leap_xu_ctrl_cb *getter;
    leap_xu_ctrl_cb *setter;
};

int set_leap_xu_strobe_width(void *fh, const struct leap_xu_ctrl *xu_ctrl, void *data){
    printk("Set strobe width to %lu\n", *(unsigned long *)data);
    return 0;
}

int get_leap_xu_strobe_width(void *fh, const struct leap_xu_ctrl *xu_ctrl, void *data){
    uint32_t strobe_width = 0xff0000ff;
    *(uint32_t *)data = strobe_width;
    return 0;
}

int get_leap_xu_devcaps(void *fh, const struct leap_xu_ctrl *xu_ctrl, void *data){
    LEAP_DEVCAPS *dev_caps = (LEAP_DEVCAPS *)data;
    dev_caps->flags = 0x01010101;
    dev_caps->firmware_rev = 1;
    dev_caps->controller_id = CONTROLLER_OV580;
    dev_caps->sensor_id = SENSOR_OV7251;
    strcpy(dev_caps->serial, "one");
    return 0;
}

static struct leap_xu_ctrl leap_xu_ctrls[] = {
    {LEAP_XU_STROBE_WIDTH, sizeof(uint32_t), &get_leap_xu_strobe_width, &set_leap_xu_strobe_width},
    {LEAP_XU_DEVCAPS, sizeof(uint8_t) * sizeof(LEAP_DEVCAPS), &get_leap_xu_strobe_width, NULL},
};
static const int num_leap_xu_ctrls = sizeof( leap_xu_ctrls ) / sizeof( leap_xu_ctrls[0] );

static long handle_xu_operation(void *fh, bool valid_prio, struct uvc_xu_control_query *xu_query){
    struct leap_xu_ctrl *xu_ctrl = NULL;
    int c;
    
    //find the control struct
    for(c =0; c < num_leap_xu_ctrls; c++){
        if(leap_xu_ctrls[c].selector == xu_query->selector){
            xu_ctrl = &leap_xu_ctrls[c];
            break;
        }
    }
    
    if(xu_ctrl == NULL){
        return -EINVAL;
    } else {
        switch(xu_query->query){
            case UVC_SET_CUR:
                if(xu_ctrl->setter){
                    return xu_ctrl->setter(fh, xu_ctrl, xu_query->data);
                } else {
                    return -EBADRQC;
                }
            case UVC_GET_CUR:
                if(xu_ctrl->getter){
                    return xu_ctrl->getter(fh, xu_ctrl, xu_query->data);
                } else {
                    return -EBADRQC;
                }
            case UVC_GET_LEN:
                *xu_query->data = xu_ctrl->dataSize;
                return 0;
            case UVC_GET_INFO:
                *(xu_query->data) = 0;
                if(xu_ctrl->getter) *(xu_query->data) |= 0x01;
                if(xu_ctrl->setter) *(xu_query->data) |= 0x02;
                return 0;
            default:
                return -EINVAL;
        }
    }
}

long leap_xu_ioctl_default(struct file *file, void *fh, bool valid_prio, unsigned int cmd, void *arg) {
         if(cmd == UVCIOC_CTRL_QUERY){
            return handle_xu_operation(fh, valid_prio, arg);
         } else {
            return -ENOTTY;
         }
}
