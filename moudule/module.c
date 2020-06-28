#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/version.h>

#include "./fpga_dot_font.h"


#define MODULE_MAJOR 265				
#define MODULE_NAME "fpga_control"

#define PUSH_SWITCH_ADDRESS 0x08000050
#define FND_ADDRESS 0x08000004 
#define LED_ADDRESS 0x08000016
#define DOT_ADDRESS 0x08000210

#define MAX_BUTTON 9
#define DOT_HEIGHT 10

static int module_usage = 0;
static unsigned char *switch_addr;
static unsigned char *fnd_addr;
static unsigned char *dot_addr;
static unsigned char *led_addr;

ssize_t read_callback(struct file *inode, char *gdata, size_t length, loff_t *off_what); 
ssize_t write_callback(struct file *inode, const char *gdata, size_t length, loff_t *off_what);
int open_callback(struct inode *minode, struct file *mfile);
int release_callback(struct inode *minode, struct file *mfile);

struct file_operations controll_fops =
{
	.owner =		THIS_MODULE,
	.open =		open_callback,
	.write = write_callback,
	.read  =		read_callback,	
	.release = release_callback,
};

int open_callback(struct inode *minode, struct file *mfile) 
{	
	if(module_usage != 0) return -EBUSY;

	module_usage = 1;


	return 0;
}

int release_callback(struct inode *minode, struct file *mfile) 
{
	module_usage = 0;

	return 0;
}

// write number to fnd
ssize_t write_callback(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
	unsigned char value[4];
	unsigned short int value_short = 0;
	const char *tmp = gdata;

	if (copy_from_user(&value, tmp, 4))
		return -EFAULT;

    value_short = value[0] << 12 | value[1] << 8 |value[2] << 4 |value[3];
    outw(value_short,(unsigned int)fnd_addr);	    
	
	return length;
}


// read pressed button and print button number to dot matrix and led
ssize_t read_callback(struct file *inode, char *gdata, size_t length, loff_t *off_what) 
{
	int i,j;
	unsigned char push_sw_value[MAX_BUTTON];	
    unsigned short _s_value;
    int pressed_button = 0;
    unsigned short value_short = 0;


	for(i=0;i<9;i++) 
    {
		_s_value = inw((unsigned int)switch_addr+i*2);
        push_sw_value[i] = _s_value &0xFF;
    }

	for(i=0;i<MAX_BUTTON;i++)
    {
		if (push_sw_value[i] != 0)
		{
			pressed_button = i+1;
			break;
		}
	}


    // controll other device on read

    value_short = (unsigned short)(pressed_button);

    //dot
    for(j=0;j<DOT_HEIGHT;j++)
    {            
	   outw(fpga_number[value_short][j] & 0x7F,(unsigned int)dot_addr+j*2);
    }
    // led
    outw(1<<(value_short-1)%8, (unsigned int)led_addr);


	if (copy_to_user(gdata, &pressed_button, 1))
		return -EFAULT;

	return length;	
}

int __init module_init_callback(void)
{
	int result;

	result = register_chrdev(MODULE_MAJOR, MODULE_NAME, &controll_fops);
	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}

	switch_addr = ioremap(PUSH_SWITCH_ADDRESS, 0x18);
    fnd_addr = ioremap(FND_ADDRESS, 0x4);
    led_addr = ioremap(LED_ADDRESS, 0x1);
    dot_addr = ioremap(DOT_ADDRESS, 0x10);

	printk("init module, %s major number : %d\n", MODULE_NAME, MODULE_MAJOR);

	return 0;
}

void __exit module_exit_callback(void) 
{
	iounmap(switch_addr);
    iounmap(fnd_addr);
    iounmap(led_addr);
    iounmap(dot_addr);

	unregister_chrdev(MODULE_MAJOR, MODULE_NAME);
}

module_init(module_init_callback);
module_exit(module_exit_callback);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("20141494");
