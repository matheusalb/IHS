#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("PCI Driver (CPU - FPGA)");
MODULE_AUTHOR("mvca, nss2 and others");

#define HEX1234       1
#define HEX5678       2
#define REDLEDS       3
#define GREENLEDS     4
#define SWITCHES      5
#define BUTTONS       6

//-- Hardware Handles
static void *hex1234;  // handle to 32-bit output PIO
static void *hex5678;  // handle to 32-bit output PIO
static void *redLeds;      // handle to 32-bit output PIO
static void *greenLeds;    // handle to 32-bit output PIO
static void *switches;     // handle to 16-bit input PIO
static void *buttons;      // handle to 16-bit input PIO

//-- Char Driver Interface
static int   access_count =  0;
static int   MAJOR_NUMBER = 91;

static int     char_device_open    (struct inode*, struct file*);
static int     char_device_release (struct inode*, struct file*);
static ssize_t char_device_read    (struct file*, char*, size_t, loff_t*);
static ssize_t char_device_write   (struct file*, const char*, size_t, loff_t*);

static struct file_operations file_opts = {
	.read = char_device_read,
	.open = char_device_open,
	.write = char_device_write,
	.release = char_device_release
};

static int char_device_open(struct inode *inodep, struct file *filep) {
   access_count++;
   printk(KERN_ALERT "altera_driver: opened %d time(s)\n", access_count);
   return 0;
}

static int char_device_release(struct inode *inodep, struct file *filep) {
   printk(KERN_ALERT "altera_driver: device closed.\n");
   return 0;
}

static ssize_t char_device_read(struct file *filep, char *buf, size_t option, loff_t *off){
   printk(KERN_ALERT "altera_driver: read %d bytes\n", option);
    
  switch(option){
    case BUTTONS:
        if(copy_to_user(buf,buttons,sizeof(uint32_t))) return -1;
      break;
    case SWITCHES:
        if(copy_to_user(buf,switches,sizeof(uint32_t))) return -1;
      break;
    default:return -1;
  }

  return sizeof(uint32_t);

  // unsigned data = 0;
  // switch(option) {
  //   case SWITCHES:
  //     data = ioread32(switches);
  //     break;
  //   case BUTTONS:
  //     data = ioread32(buttons);
  //     break;
  //   default:
  //     printk(KERN_ALERT "Invalid option for read()");
  //     return -1;
  // }

  // // send data from kernel space to user space
  // copy_to_user(buf, &data, sizeof(unsigned));

  // return sizeof(uint32_t);
}

static ssize_t char_device_write(struct file *filep, const char *buf, size_t option, loff_t *off) {
  // unsigned data = 0;

  // get data from the user space to kernel space
  // copy_from_user(&data, buf, sizeof(uint32_t));

  switch(option){
    case HEX1234:
        if(copy_from_user(hex1234,buf,2*sizeof(uint32_t))) return -1;
      break;
    case HEX5678:
      if(copy_from_user(hex5678,buf,2*sizeof(uint32_t))) return -1;
    break;
    case REDLEDS:
        if(copy_from_user(redLeds,buf,2*sizeof(uint32_t))) return -1;
      break;
    case GREENLEDS:
        if(copy_from_user(greenLeds,buf,2*sizeof(uint32_t))) return -1;
      break;
    default:return -1;
  }
  return sizeof(uint32_t);

  // switch (option) {
  //   case HEX1234:
  //     iowrite32(data, hex1234);
  //     break;
  //   case HEX5678:
  //     iowrite32(data, hex5678);
  //     break;
  //   case REDLEDS:
  //     iowrite32(data, redLeds);
  //     break;
  //   case GREENLEDS:
  //     iowrite32(data, greenLeds);
  //     break;
  //   default:
  //     printk(KERN_ALERT "Invalid option for write()");
  //     return -1;
  // }

  // return sizeof(uint32_t);
}

//-- PCI Device Interface
static struct pci_device_id pci_ids[] = {
  { PCI_DEVICE(0x1172, 0x0004), },
  { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id);
static void pci_remove(struct pci_dev *dev);

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
  int vendor;
  int retval;
  unsigned long resource;

  retval = pci_enable_device(dev);
  
  if (pci_get_revision(dev) != 0x01) {
    printk(KERN_ALERT "altera_driver: cannot find pci device\n");
    return -ENODEV;
  }

  pci_read_config_dword(dev, 0, &vendor);
  printk(KERN_ALERT "altera_driver: Found Vendor id: %x\n", vendor);

  resource = pci_resource_start(dev, 0);
  printk(KERN_ALERT "altera_driver: Resource start at bar 0: %lx\n", resource);

  hex1234 = ioremap_nocache(resource + 0XC000, 0x20);
  hex5678 = ioremap_nocache(resource + 0XC020, 0x20);
  redLeds  = ioremap_nocache(resource + 0XC040, 0x20);
  greenLeds  = ioremap_nocache(resource + 0XC060, 0x20);
  switches  = ioremap_nocache(resource + 0XC080, 0x20);
  buttons  = ioremap_nocache(resource + 0XC100, 0x20);
  
  return 0;
}

static void pci_remove(struct pci_dev *dev) {
  iounmap(hex1234);
  iounmap(hex5678);
  iounmap(redLeds);
  iounmap(greenLeds);
  iounmap(switches);
  iounmap(buttons);
}


//-- Global module registration
static int __init altera_driver_init(void) {
   int t = register_chrdev(MAJOR_NUMBER, "de2i150_altera", &file_opts);
   t = t | pci_register_driver(&pci_driver);

   if(t<0)
      printk(KERN_ALERT "altera_driver: error: cannot register char or pci.\n");
   else
     printk(KERN_ALERT "altera_driver: char+pci drivers registered.\n");

   return t;
}

static void __exit altera_driver_exit(void) {
  printk(KERN_ALERT "Goodbye from de2i150_altera.\n");

  unregister_chrdev(MAJOR_NUMBER, "de2i150_altera");
  pci_unregister_driver(&pci_driver);
}

module_init(altera_driver_init);
module_exit(altera_driver_exit);
