#include "file_ops.h"
#include "device_specific.h"
#include "pcie_ctrl.h"

#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>

//Fill in the file oerations structure will the various functions
//defined in this file.
const struct file_operations file_ops = {
    .owner = THIS_MODULE,
    .read = read,
    .write = write,
    .llseek = llseek,
    .open = open,
    .release = release,
};

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
ssize_t read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
    //The way that this works is that the offset is not "remembered" between function calls
    unsigned int val;
    unsigned long not_copied_count;

    if(!access_ok(buff, count)) {
        printk(KERN_INFO "Read buffer error\n");
        return -1;
    }

    printk(KERN_INFO "READ\n");

    //Read a 32bit value at the offset stored in offp with in BAR0
    val = ioread32(bar0_ptr + *offp);

    //The offset value is stored between operations
    *offp += count;

    //Copy the data back to user space and return the number of bytes that could
    //be copied successfully.
    not_copied_count = copy_to_user(buff, &val, sizeof(unsigned int));
    return (count - not_copied_count);
}

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
ssize_t write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {

    unsigned long not_copied_count;
    u32 *kernel_ptr;

    //Check that the userspace buffer is valid
    if(!access_ok(buff, count)) {
        printk(KERN_INFO "Write buffer error\n");
        return -1;
    }
    
    //Copy the userspace buffer to a kernel space buffer
    kernel_ptr = (u32*) kmalloc(count * sizeof(char), GFP_KERNEL);
    not_copied_count = copy_from_user(kernel_ptr, buff, count);

    printk(KERN_INFO "WRITE\n");

    //Read a 32bit value at the offset stored in offp with in BAR0
    iowrite32(kernel_ptr[0], bar0_ptr + *offp);

    //Free temporary kernel space buffer
    kfree(kernel_ptr);

    *offp += count;

    return (count - not_copied_count);
}


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
int open (struct inode *inode, struct file *filp) {
    //Nothing to be done here. Just log that the file was
    //opened and return.
    printk(KERN_INFO "File Opened\n");

    return 0;
}

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
int release(struct inode *inode, struct file *filp) {
    //Nothing to be done here. Just log that the file was
    //closed and return.
    printk(KERN_INFO "File Closed\n");

    return 0;
}

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
loff_t llseek(struct file *filp, loff_t offset, int whence) {

    //Set the offset relative to the start (In absolute terms).
    if(whence == SEEK_SET) {
        filp->f_pos = offset;
    }
    //Set the offset relative to the current position
    else if(whence == SEEK_CUR){
        filp->f_pos += offset;
    }

    printk(KERN_INFO "SEEK\n");
    printk(KERN_INFO "SEEK CURRENT: %lld\n", filp->f_pos);

    return filp->f_pos;
}

