/*
 * Copyright © 2016 Leap Motion, Inc.
 *
 */

#ifndef LEAP_XU_V4L2_H
#define LEAP_XU_V4L2_H

#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-event.h>
#include <media/v4l2-common.h>

long leap_xu_ioctl_default(struct file *file, void *fh, bool valid_prio, unsigned int cmd, void *arg);

#endif /* LEAP_XU_V4L2_H */
