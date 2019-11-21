/* Altera DE2I-150 FPGA Driver for Linux made by Matheus Souza (github.com/mfbsouza)
   for the MusicBox Project, A Music Instrument Simulator using Arduino Uno and FPGA */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_DESCRIPTION("FPGA Driver for MusicBox Project");
MODULE_AUTHOR("Matheus Souza");

#define HEX1234       1
#define HEX5678       2
#define REDLEDS       3
#define GREENLEDS     4
#define SWITCHES      5
#define BUTTONS       6

/* General Purpose Variables for Driver */
static uint32_t last_buttons = 0;
static uint32_t last_switches = 0;
static uint32_t data;
static int flag = 0;
static int access_count = 0;

// --- Pointers to FPGA IO hardware
static void *hex1234;  // handle to 32-bit output PIO
static void *hex5678;  // handle to 32-bit output PIO
static void *redLeds;      // handle to 32-bit output PIO
static void *greenLeds;    // handle to 32-bit output PIO
static void *switches;     // handle to 16-bit input PIO
static void *buttons;      // handle to 16-bit input PIO

/* --- CHAR DEVICE INFORMATION & FUNCTIONS --- */
static int MAJOR_NUMBER = 91;
static int     dev_open     (struct inode *, struct file *);
static int     dev_release  (struct inode *, struct file *);
static ssize_t dev_read     (struct file * , char *      , size_t, loff_t *);
static ssize_t dev_write    (struct file * , const char *, size_t, loff_t *);

static struct file_operations file_opts = {
    .read    = dev_read,
    .open    = dev_open,
    .write   = dev_write,
    .release = dev_release
};

/* Char Driver Functions implementations */

static int dev_open(struct inode *inodep, struct file *filep) {
    access_count++;
    printk(KERN_ALERT "FPGA device opened %d time(s)\n", access_count);
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_ALERT "FPGA device closed.\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buf, size_t opt, loff_t *off) {
    data = 0;
    
    switch(opt) {
        case HEX5678:
            data = ioread32(hex5678);
            break;
        case HEX1234:
            data = ioread32(hex1234);
            break;
        case SWITCHES:
            data = ioread32(switches);
            if(data != last_switches) {
                flag = 1;
            }
            last_switches = data;
            break;
        case BUTTONS:
            data = ioread32(buttons);
            if(data != last_buttons && data != 15) {
                flag = 1;
            }
            last_buttons = data;
            break;
        case GREENLEDS:
            data = ioread32(greenLeds);
            break;
        case REDLEDS:
            data = ioread32(redLeds);
            break;
        default:
            printk(KERN_ALERT "Invalid Option from Read().\n");
            return -1; // send error to user space
    }

    if(flag == 1) {
        // send data to user space
        copy_to_user(buf, &data, sizeof(uint32_t));
        printk(KERN_ALERT "SEND: %d", data);
        flag = 0; // reset flag
        return 4; // red 4 bytes
    } else {
        return 0; // red 0 bytes
    }
}

static ssize_t dev_write(struct file *filep, const char *buf, size_t opt, loff_t *off) {
    data = 0;
    // get data from the user space to kernel space
    copy_from_user(&data, buf, sizeof(uint32_t));

    switch(opt) {
        case HEX5678:
            iowrite32(data, hex5678);
            break;
        case HEX1234:
            iowrite32(data, hex1234);
            break;
        case GREENLEDS:
            iowrite32(data, greenLeds);
            break;
        case REDLEDS:
            iowrite32(data, redLeds);
            break;
        default:
            printk(KERN_ALERT "Invalid Option from Write().\n");
            return -1;
    }
    printk(KERN_ALERT "WROTE: %d", data);
    return 4;
}

/* --- PCI INTERFACE INFORMATION & FUNCTIONS --- */
static int  pci_probe  (struct pci_dev *dev, const struct pci_device_id *id);
static void pci_remove (struct pci_dev *dev);

static struct pci_device_id pci_ids[] = {
    { PCI_DEVICE(0x1172, 0x0004), },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static struct pci_driver pci_driver = {
    .name     = "alterahello",
    .id_table = pci_ids,
    .probe    = pci_probe,
    .remove   = pci_remove,
};

static unsigned char pci_get_revision(struct pci_dev *dev) {
    u8 revision;

    pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
    return revision;
}

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id) {
    int vendor, retval;
    unsigned long resource;

    retval = pci_enable_device(dev);
  
    if (pci_get_revision(dev) != 0x01) {
        printk(KERN_ALERT "ERROR: cannot find PCI device\n");
        return -ENODEV;
    }

    pci_read_config_dword(dev, 0, &vendor);
    printk(KERN_ALERT "PCI Device Found. Vendor: %x\n", vendor);

    resource = pci_resource_start(dev, 0);
    printk(KERN_ALERT "PCI device resources start at bar 0: %lx\n", resource);

    // hex1234 = ioremap_nocache(resource + 0XC000, 0x20);
    // hex5678 = ioremap_nocache(resource + 0XC020, 0x20);
    // redLeds  = ioremap_nocache(resource + 0XC040, 0x20);
    // greenLeds  = ioremap_nocache(resource + 0XC060, 0x20);
    // switches  = ioremap_nocache(resource + 0XC080, 0x20);
    // buttons  = ioremap_nocache(resource + 0XC100, 0x20);

    hex1234  = ioremap_nocache(resource + 0XC000, 0x20);
    hex5678  = ioremap_nocache(resource + 0XC140, 0x20);
    switches  = ioremap_nocache(resource + 0XC040, 0x20);
    buttons  = ioremap_nocache(resource + 0XC080, 0x20);
    greenLeds = ioremap_nocache(resource + 0XC0F0, 0x20);
    redLeds = ioremap_nocache(resource + 0XC0B0, 0x20);

    return 0;
}

static void pci_remove(struct pci_dev *dev) {
    iounmap(hex1234);
    iounmap(hex5678);
    iounmap(switches);
    iounmap(buttons);
    iounmap(greenLeds);
    iounmap(redLeds);
}

/* --- Driver Registration --- */
static int __init altera_driver_init(void) {
    int t = register_chrdev(MAJOR_NUMBER, "de2i150_altera", &file_opts);
    t = t | pci_register_driver(&pci_driver);

    if(t<0)
        printk(KERN_ALERT "ERROR: cannot register char or pci.\n");
    else
        printk(KERN_ALERT "PCI Device registred.\n");
    return t;
}

static void __exit altera_driver_exit(void) {
    printk(KERN_ALERT "Closing Driver...\n");
    unregister_chrdev(MAJOR_NUMBER, "de2i150_altera");
    pci_unregister_driver(&pci_driver);
}

module_init(altera_driver_init);
module_exit(altera_driver_exit);