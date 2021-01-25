#include "pcie_ctrl.h"
#include "file_ops.h"

//Add data about supported devices to the module table so the kernel
//knows what devices this drives should be paired with.
MODULE_DEVICE_TABLE(pci, pci_id_array);

//Pointer to the start of the BAR0 address space AFTER it has been 
//mapped into the virtual address space.
char *bar0_ptr;
//Data related to bar 0 on the device
unsigned long bar0_size;
unsigned long bar0_start;

u8 interrupt_number;

/*
    This function is called when the kernel finds a device that can be
    paired with the driver.

    Parameters:
        dev     -> Device structure pointer of the device the driver is
                   being paired with.
        id      -> Pointer to the ID information of the device being 
                   paired.
    Return:
        0 on success and a negative value on failure.
*/
int pci_probe (struct pci_dev *dev, const struct pci_device_id *id) {
    int status;
    u16 vendor_id;

    //Store the addres of both the start and end of the PCIe memory region
    unsigned long bar0_ptr_int_start;
    unsigned long bar0_ptr_int_end;

    printk(KERN_INFO "PCI PROBE\n");
    status = pci_enable_device(dev);

    if(status != 0) {
        return status;
    }

    //Read the vendor ID from the configuration space of the device.
    status = pci_read_config_word(dev, PCI_VENDOR_ID, &vendor_id);

    if(status != 0) {
        return status;
    }

    //All PCI values are big endian so the conversion to the cpu byte ordering
    //is required to make sure this works on all platforms.
    printk(KERN_INFO "%d\n", be16_to_cpu(vendor_id));

    //Get the start and end addresses of the devices BAR0 memory region.
    bar0_ptr_int_start = pci_resource_start(dev, 0);
    bar0_ptr_int_end = pci_resource_end(dev, 0);

    bar0_size = bar0_ptr_int_end - bar0_ptr_int_start;
    bar0_start = bar0_ptr_int_start;

    //Map the BAR0 memory region of the device into the virtual address space.
    bar0_ptr = (char*) ioremap(bar0_ptr_int_start, bar0_size);

    return 0;
}

/*
    This function is called when the device is removed.

    Paramaters:
        dev     -> Pointer to the device the driver is paired with

    Return:
        Nothing.
*/
void pci_remove (struct pci_dev *dev) {
    //Un-map BAR0
    iounmap(bar0_ptr);
    //Disable the device
    pci_disable_device(dev);
    printk(KERN_INFO "PCI REMOVE\n");
}