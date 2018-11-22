/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>

//#include <linux/platform_device.h>
//#include <linux/ioport.h>
//#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/fs.h>
//#include <linux/>
//#include <linux/>



/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

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

static int __init template_init(void)
{

	struct cdev my_cdev;

	cdev_init(&my_cdev, &gamepad_fops);

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

