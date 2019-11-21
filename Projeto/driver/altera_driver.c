#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("PCI Driver (CPU - FPGA)");
MODULE_AUTHOR("lema, mvca, nss2 and others");


//-- Enums
enum OPS {HEX_RIGHT,HEX_LEFT,RED_LEDS,GREEN_LEDS,BUTTONS,SWITCHES};

//-- Hardware Handles
static void * hex_right;  // handle to 32-bit output PIO
static void * hex_left;
static void * red_leds;
static void * green_leds;
static void * buttons;
static void * switches;

//-- Char Driver Interface
static int   access_count =  0;
static int   MAJOR_NUMBER = 91;

static int     char_device_open    ( struct inode * , struct file *);
static int     char_device_release ( struct inode * , struct file *);
static ssize_t char_device_read    ( struct file * , char *,       size_t , loff_t *);
static ssize_t char_device_write   ( struct file * , const char *, size_t , loff_t *);

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

static ssize_t char_device_read(struct file *filep, char *buf, size_t opt, loff_t *off) {
  printk(KERN_ALERT "altera_driver: read %d bytes\n", opt);
    
  switch(opt){
		case BUTTONS:
				if(copy_to_user(buf,buttons,sizeof(uint32_t))) return -1;
			break;
		case SWITCHES:
				if(copy_to_user(buf,switches,sizeof(uint32_t))) return -1;
			break;
		default:return -1;
	}

  return sizeof(uint32_t);
}

static ssize_t char_device_write(struct file *filep, const char *buf, size_t opt, loff_t *off) {
	printk(KERN_ALERT "altera_driver: write %d bytes\n", sizeof(uint32_t));
	printk(KERN_ALERT "adress hexport pointer: %p \n" ,hex_right);
	printk(KERN_ALERT "adress red_leds pointer: %p \n", red_leds);
	printk(KERN_ALERT "adress display pointer: %p \n",hex_left);
	
	switch(opt){
		case HEX_RIGHT:
				if(copy_from_user(hex_right,buf,2*sizeof(uint32_t))) return -1;
			break;
		case HEX_LEFT:
				if(copy_from_user(hex_left,buf,2*sizeof(uint32_t))) return -1;
			break;
		case RED_LEDS:
				if(copy_from_user(red_leds,buf,2*sizeof(uint32_t))) return -1;
			break;
		case GREEN_LEDS:
				if(copy_from_user(green_leds,buf,2*sizeof(uint32_t))) return -1;
			break;
		default:return -1;
	}
	return sizeof(uint32_t);
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

	hex_right = ioremap_nocache(resource + 0xC000, 0x20);
	hex_left = ioremap_nocache(resource + 0xC080, 0x20);
	red_leds = ioremap_nocache(resource + 0xC100,0x20);
	green_leds = ioremap_nocache(resource + 0xC0C0,0x20);
	buttons = ioremap_nocache(resource + 0xC140,0x20);
	switches = ioremap_nocache(resource + 0xC180,0x20);
	return 0;
}

static void pci_remove(struct pci_dev *dev) {
  iounmap(hex_right);
  iounmap(hex_left);
  iounmap(red_leds);
  iounmap(green_leds);
  iounmap(buttons);
  iounmap(switches);
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
