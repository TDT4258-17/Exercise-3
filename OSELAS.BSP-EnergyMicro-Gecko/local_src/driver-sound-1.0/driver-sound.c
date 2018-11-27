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

static uint32_t* dac_remap;
static uint32_t* timer_remap;
static struct class *cl;
static struct cdev my_cdev;
static dev_t dev;
static unsigned char val = 65;

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
	request_mem_region(0x40004000, 0x02c, "Sound");
	dac_remap = ioremap_nocache(0x40004000, 0x02c);


	request_mem_region(0x40010c00, 0x028, "Sound");
	timer_remap = ioremap_nocache(0x40010c00, 0x28);

	// configure dac here
	dac_remap[0] = 0x50010;
	dac_remap[2] = 1;
	dac_remap[3] = 1;



	error  = alloc_chrdev_region(&dev, 0, 1, "Sound");
	
	if(error < 0){
		printk("ERROR Allocating character device in sound driver");
	}	
	
	cdev_init(&my_cdev, &sound_fops);
	
	error = cdev_add(&my_cdev, dev, 1);
	if (error < 0)
	{
		printk("ERROR Adding character device?\n");
	}
	
	cl = class_create(THIS_MODULE, "sound");
	device_create(cl, NULL, dev, NULL, "sound");




	printk("Loaded Sound driver\n");
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
	device_destroy(cl, dev);
	class_destroy(cl);
	printk("Unladed Sound driver\n");
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
	dac_remap[8] = val;
	dac_remap[9] = val;
	if (val == 65)
		val = 0;
	else
		val = 65;
	return 0;
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Sound module.");
MODULE_LICENSE("GPL");

