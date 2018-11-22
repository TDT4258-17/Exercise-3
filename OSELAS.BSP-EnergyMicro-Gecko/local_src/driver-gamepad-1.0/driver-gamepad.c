/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>

//#include <linux/platform_device.h>
//#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
//#include <linux/>
//#include <linux/>

#include "efm32gg.h"

static char* gpio_remap;
static struct class *cl;
static struct cdev my_cdev;
static dev_t dev;

static int gamepad_open(struct inode *inode, struct file *filp);
static int gamepad_release(struct inode *inode, struct file *filp);
static int gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

static struct file_operations gamepad_fops = 
{
	.owner = THIS_MODULE,
	.open = gamepad_open,
	.release = gamepad_release,
	.read = gamepad_read,
	.write = gamepad_write
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
	//struct resource* mem_resource = request_mem_region(0x40006048, 20, "CharDevice");
	request_mem_region(0x40006048, 0x20, "CharDevice");

	gpio_remap = ioremap_nocache(0x40006048, 0x20);


	//*GPIO_PC_MODEL = 0x33333333;
	gpio_remap[4] = 0x33;
	gpio_remap[5] = 0x33;
	gpio_remap[6] = 0x33;
	gpio_remap[7] = 0x33;

	//*GPIO_PC_DOUT = 0xff;
	gpio_remap[12] = 0xff;


	int error;
	
	error = alloc_chrdev_region(&dev, 0, 1, "CharDevice");
	if (error < 0)
	{
		printk("ERROR Allocating character device?\n");
	}

	cdev_init(&my_cdev, &gamepad_fops);

	error = cdev_add(&my_cdev, dev, 1);
	if (error < 0)
	{
		printk("ERROR Adding character device?\n");
	}

	cl = class_create(THIS_MODULE, "gamepad");
	device_create(cl, NULL, dev, NULL, "gamepad");



	printk("Hello World, here is your module speakingg\n");
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
	device_destroy(cl, dev);
	class_destroy(cl);

	printk("Short life for a small module...\n");
}

static int gamepad_open(struct inode *inode, struct file *filp)
{
	return 0;
}
static int gamepad_release(struct inode *inode, struct file *filp)
{
	return 0;
}
static int gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	*buff = ~(gpio_remap[0x1c]);
	return 0;
}
static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	return 0;
}

module_init(template_init);
module_exit(template_cleanup);
//module_init(__init);
//module_exit(__exit);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

