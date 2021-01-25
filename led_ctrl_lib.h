////////////////////////////////////////////////////
//Low-level API
////////////////////////////////////////////////////

/*
    Write zero to each of the user writable registers on the device.

    Parameters:
        fd  -> File descriptor of the device file.
*/
int clear_registers(int fd);

/*
    Reads the value of the register at a given offset.

    Paramaters:
        fd          -> File descriptor of the device file.
        reg_offset  -> Offset of the register to be read.

    Return:
        32bit register value read from the device.
*/
int read_register(int fd, int reg_offset, uint32_t *value);

/*
    Writes a given value to the register at a given offset.

    Paramaters:
        fd          -> File descriptor of the device file.
        reg_offset  -> Offset of the register to be written to.

    Return:
        32bit register value read from the device.
*/
int write_register(int fd, int reg_offset, uint32_t value);