#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <linux/fs.h>
#include <linux/completion.h>

/*
    Function to read data from the PCIe device's BAR0. NOTE: For simplicity this function
    only reads a single 32bit value regardless of how large the provided buffer is.

    Paramaters:
        filep   -> Pointer to the devices file sturcture.
        buff    -> User space buffer from user space. Cannont be directly accessed
                   in kernel space.
        count   -> Indicates the size of the buffer pointed to by buff.
        offp    -> Offset to read from with in the file. In this case this is
                   the offset to read from with in BAR0.

    Return:
        Returns the number of bytes read during the operation.
*/
ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp);

/*
    Function to write data to the PCIe device's BAR0. NOTE: For simplicity this function
    only writes a single 32bit value regardless of how large the provided buffer is.

    Paramaters:
        filep   -> Pointer to the devices file sturcture.
        buff    -> User space buffer from user space. Cannont be directly accessed
                   in kernel space.
        count   -> Indicates the size of the buffer pointed to by buff.
        offp    -> Offset to write to from with in the file. In this case this is
                   the offset to write to with in BAR0.

    Return:
        Returns the number of bytes written during the operation.
*/
ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

/*
    Function to let user space programs open the driver's device file.

    Paramaters:
        inode   -> Pointer to the devices inode sturcture
        filep   -> Pointer to the devices file sturcture
        NOTE: For more detials on these parametrs look up the specific
        structure types.

    Return:
        0 on success negative value on failure.
*/
int open (struct inode *inode, struct file *filp);

/*
    Function to let user space programs close the driver's device file.

    Paramaters:
        inode   -> Pointer to the devices inode sturcture
        filep   -> Pointer to the devices file sturcture
        NOTE: For more detials on these parametrs look up the specific
        structure types.

    Return:
        0 on success negative value on failure.
*/
int release(struct inode *inode, struct file *filp);

/*
    Allows to set the offset that will be written to or read from.

    Paramaters:
        filep   -> Pointer to the devices file sturcture.
        offset  -> The value used to set the new offset
        whence  -> Indicates where the offset should be set from
                   The options are SEEK_SET or SEEK_CUR. When SEEK_SET
                   is used the offset is set relative to the start
                   position. When SEEK_CUR is used the offset is 
                   set from the current position.

    Return:
        Returns the newly set offset value.
*/
loff_t llseek(struct file *filp, loff_t offset, int whence);

//This structure holds all of the file operations that the the driver supports
extern const struct file_operations file_ops;

#endif