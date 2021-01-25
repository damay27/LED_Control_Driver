#!/bin/bash

DRIVER_NAME=led_ctrl
DEVICE_FILE_NAME=led_ctrl

./device_refresh.sh

#Remove the driver if it is already loaded
rmmod $DRIVER_NAME
#Rebuild the driver
make
#Reload the driver once the build has finished
insmod $DRIVER_NAME.ko

MAJOR_NUMBER=`cat /proc/devices | grep $DRIVER_NAME | awk '{print $1}'`

#The c argument creates a character device
mknod /dev/$DEVICE_FILE_NAME c $MAJOR_NUMBER 0
