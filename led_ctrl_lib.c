#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "device_specific.h"

////////////////////////////////////////////////////
//Low-level API
////////////////////////////////////////////////////

/*
    Write zero to each of the user writable registers on the device.

    Parameters:
        fd  -> File descriptor of the device file.
*/
int clear_registers(int fd) {
    uint32_t data = 0;
    int status;

    //Move the file pointer to the start flag register then write zero to it
    //and check for errors.
    lseek(fd, 0, SEEK_SET);
    status = write(fd, &data, sizeof(data));
    if(status == -1) return -1;

    return 0;
}

/*
    Reads the value of the register at a given offset.

    Paramaters:
        fd          -> File descriptor of the device file.
        reg_offset  -> Offset of the register to be read.

    Return:
        32bit register value read from the device.
*/
int read_register(int fd, int reg_offset, uint32_t *value) {
    int read_count;

    //Move to the offset of the register
    lseek(fd, reg_offset, SEEK_SET);

    //Read the value
    read_count = read(fd, value, sizeof(uint32_t));
    //Check that the correct amount of data was read. (32 bit == 4 bytes)
    if(read_count != 4) return -1;

    return 0;
}

/*
    Writes a given value to the register at a given offset.

    Paramaters:
        fd          -> File descriptor of the device file.
        reg_offset  -> Offset of the register to be written to.

    Return:
        32bit register value read from the device.
*/
int write_register(int fd, int reg_offset, uint32_t value) {
    
    int write_count;

    //Move to the correct register offset
    lseek(fd, reg_offset, SEEK_SET);
    //Write the value
    write_count = write(fd, &value, sizeof(uint32_t));

    //Check that the correct amount of data was written. (32 bit == 4 bytes)
    if(write_count != 4) return -1;
    else return 0;

}
