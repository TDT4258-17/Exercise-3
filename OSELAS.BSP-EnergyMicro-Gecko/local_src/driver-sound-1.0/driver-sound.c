/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>

#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>

static uint32_t* dac_remap;
static uint32_t* timer_remap;
static struct class *cl;
static struct cdev my_cdev;
static dev_t dev;

static unsigned char val = 32;
static unsigned char duration = 0;
static unsigned short period = 0;
static unsigned short top = 0;
static unsigned char special = 0;

static int sound_open(struct inode *inode, struct file *filp);
static int sound_release(struct inode *inode, struct file *filp);
static int sound_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t sound_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

static struct file_operations sound_fops = 
{
	.owner = THIS_MODULE,
	.open = sound_open,
	.release = sound_release,
	.read = sound_read,
	.write = sound_write
};

irqreturn_t timer3_handler(int irq, void* dev_id)
{
	timer_remap[6] = timer_remap[4];
	if (special)
	{
		top = (top * 50);
		top = (top / 51);
		timer_remap[7] = top;
	}
	dac_remap[8] = val;
	dac_remap[9] = val;

	if (val == 32)
		val = 0;
	else
		val = 32;

	period++;

	if (period > duration)
	{
		period = 0;
		//*TIMER1_CMD = 2;		// stop timer1
		timer_remap[1] = 2;
		special = 0;
	}

	return 0;
}

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init template_init(void)
{
	int error;

	//0x40004000 = DAC0_BASE2
	request_mem_region(0x40004000, 0x02c, "sdDev");
	dac_remap = ioremap_nocache(0x40004000, 0x02c);


	request_mem_region(0x40010c00, 0x028, "sdDev");
	timer_remap = ioremap_nocache(0x40010c00, 0x28);

	// configuring DAC
	dac_remap[0] = 0x50010;
	dac_remap[2] = 1;
	dac_remap[3] = 1;

	// configuring TIMER3
	//*TIMER1_CTRL &= 0xf0ffffff;
	//*TIMER1_CTRL |= 0x05000000;
	timer_remap[0] &= 0xf0ffffff;
	timer_remap[0] |= 0x07000000; // HFPERCLK divided by 2^7=128 => 109.375 kHz

	//	*TIMER1_TOP = 125;		// => 875 Hz frequency
	//	*TIMER1_TOP = 875;		// => 125 Hz frequency
	//	*TIMER1_TOP = 1750;		// => 62.5 Hz frequency
	timer_remap[7] = 0xffff;
	
	//*TIMER1_IEN = 1;
	timer_remap[3] = 1;


	error  = alloc_chrdev_region(&dev, 0, 1, "sdDev");
	if(error < 0){
		printk(KERN_ALERT "ERROR Allocating character device in sound driver");
	}	
	
	cdev_init(&my_cdev, &sound_fops);
	
	error = cdev_add(&my_cdev, dev, 1);
	if (error < 0)
	{
		printk(KERN_ALERT "ERROR Adding character device?\n");
	}
	
	cl = class_create(THIS_MODULE, "sound");
	device_create(cl, NULL, dev, NULL, "sound");


	error = request_irq(19, timer3_handler, 0, "sdDev", 0);
	if (error < 0)
		printk(KERN_ALERT "ERROR requesting GPIO interrupt 17?\n");

	//*TIMER1_IFC = *TIMER1_IF;
	timer_remap[6] = timer_remap[4];

	printk(KERN_INFO "Loaded Sound driver\n");
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	dac_remap[0] = 0x10;
	dac_remap[2] = 0;
	dac_remap[3] = 0;

	timer_remap[0] = 0;
	timer_remap[1] = 2;
	timer_remap[3] = 0;
	timer_remap[7] = 0xffff;

	free_irq(19, 0);
	device_destroy(cl, dev);
	class_destroy(cl);
	printk(KERN_INFO "Unlaoded Sound driver\n");
}

static int sound_open(struct inode *inode, struct file *filp)
{
	return 0;
}
static int sound_release(struct inode *inode, struct file *filp)
{
	return 0;
}
static int sound_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	return 0;
}

static ssize_t sound_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	int a = *buff;

	switch (a)
	{
	case 0: // button
		top = 500;
		duration = 50;
		timer_remap[7] = top;
		break;
	case 1: // map change
		top = 875;
		duration = 80;
		timer_remap[7] = top;
		break;
	case 2: // falling into hole
		top = 1300;
		duration = 50;
		timer_remap[7] = top;
		break;
	case 3: // special case
		top = 1300;
		duration = 150;
		special = 1;
		timer_remap[7] = top;
		break;
	}

	//*TIMER1_CMD = 1;		// start timer1
	timer_remap[1] = 1;

	return 0;
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Sound module.");
MODULE_LICENSE("GPL");

