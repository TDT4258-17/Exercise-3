/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include "efm32gg.h"
/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */
 
 
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

static int __init template_init(void)
{
	
	//0x40004000 = DAC0_BASE2
	request_mem_region(0x40004000, 0x024, "CharDevice");
	sound_remap = ioremap_nocache(0x40004000, 0x024);
	
	
		
	printk("Hello World, here is your module speaking\n");
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
	 printk("Short life for a small module...\n");
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
	return 0;
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

