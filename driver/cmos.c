#include "linux/fs.h"

struct coms_dev{
	unsigned int size;
	int bank_number;
	struct cdev cdev;
	char name[10];
}*cmos_devp;

static struct file_operations cmos_fops={
	.owner=THIS_MODULE,
	.open=cmos_open,
	.release=cmos_release,
	.read=cmos_read,
	.write=cmos_write,
	.llseek=cmos_llseek,
	.ioctl=cmos_ioctl,
};

static dev_t cmos_dev_number;
struct class *cmos_class;

#define NUM_CMOS_BANKS 2
#define CMOS_BANK_SIZE (0xFF*8)
#define DEVICE_NAME "cmos"
#define CMOS_BANK0_INDEX_PORT 0x70
#define CMOS_BANK0_DATA_PORT 0x71
#define CMOS_BANK1_INDEX_PORT 0x72
#define CMOS_BANK1_DATA_PORT 0x73

unsigned char addrports[NUM_CMOS_BANKS]={CMOS_BANK0_INDEX_PORT,CMOS_BANK1_INDEX_PORT,};
unsigned char dataports[NUM_CMOS_BANKS]={CMOS_BANK0_DATA_PORT,CMOS_BANK1_DATA_PORT,};

int __init cmos_init(void){
	int i;
	if(alloc_chrdev_region(&cmos_dev_number,0,NUM_CMOS_BANKS,DEVICE_NAME)<0){
		printk(KERN_DEBUG "Can't register device\n");
		return -1;
	}
	cmos_class=class_create(THIS_MODULE,DEVICE_NAME);

	for(i=0;i<NUM_CMOS_BANKS;i++){
		cmos_devp=kmalloc(sizeof(struct cmos_dev),GFP_KERNEL);
		if(!cmos_devp){
			printk("Bad kmalloc\n");
			return 1;
		}
	}
}

