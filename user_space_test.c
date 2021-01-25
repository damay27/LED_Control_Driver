#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

#include "led_ctrl_lib.h"


#include <sys/ioctl.h>

int main(int argc, char *argv[]) {
    int count;

    //Open the device file and check that it was opened correctly
    int fd = open("/dev/led_ctrl", O_RDWR);
    if(fd < 0) {
        printf("Failed to open device file\n");
        return -1;
    }


    //Clear all of the registers on the device and then start the prime number search
    clear_registers(fd);

    while(1) {
        uint32_t write_value, read_value;
        int status;

        //Get user input
        printf("Enter a value for the LEDs: ");
        scanf("%u", &write_value);

        //Write the value to the device register
        status = write_register(fd, 0, write_value);
        if(status != 0) {
            printf("Error writing the value to the device.\n");
            return -1;
        }

        //Read the value back from the device register
        status = read_register(fd, 0, &read_value);
        if(status != 0) {
            printf("Error reading the value from the device.\n");
            return -1;
        }
        printf("LEDs set to %u\n", read_value);
    }

    
}