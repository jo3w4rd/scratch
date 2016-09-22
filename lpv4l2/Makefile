obj-m += lpv4l2.o

lpv4l2-objs := lpv4l2-camera.o controls.o mmal-vchiq.o leap_xu_v4l2.o

EXTRA_CFLAGS += -Idrivers/misc/vc04_services -Idrivers/misc/vc04_services/interface/vcos/linuxkernel -D__VCCOREVER__=0x04000000
EXTRA_CFLAGS  += -I"drivers/misc/vc04_services/interface/vchi"
EXTRA_CFLAGS  += -I"drivers/misc/vc04_services/interface/vchiq_arm"