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
#include <linux/interrupt.h>

#include "efm32gg.h"

static uint32_t* gpio_remap_pc;
static uint32_t* gpio_remap_int;
static struct class *cl;
static struct cdev my_cdev;
static dev_t dev;
static unsigned char portC_din;

static void dummmyFunction(unsigned char a);
static void (*func_p)(unsigned char) = &dummmyFunction;

static int gamepad_open(struct inode *inode, struct file *filp);
static int gamepad_release(struct inode *inode, struct file *filp);
static int gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
//static int gamepad_ioctl(struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg);


static struct file_operations gamepad_fops = 
{
	.owner = THIS_MODULE,
	.open = gamepad_open,
	.release = gamepad_release,
	.read = gamepad_read,
	.write = gamepad_write,
	//.ioctl = gamepad_ioctl
};

static void dummmyFunction(unsigned char a)
{
	printk("D\n");
}

irqreturn_t gpio_handler(int irq, void* dev_id)
{
	
	gpio_remap_int[7] = gpio_remap_int[5];
	//printk("GP-INT\n");
	portC_din = ~(gpio_remap_pc[7]);
	(*func_p)(portC_din);
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
	int error = 0;
	
	//struct resource* mem_resource = request_mem_region(0x40006048, 20, "CharDevice");
	request_mem_region(0x40006048, 0x20, "gpDev");
	if (error < 0)
		printk(KERN_ALERT "ERROR Requestig memory region 1 for GPIO driver?\n");
	request_mem_region(0x40006100, 0x1c, "gpDev");
	if (error < 0)
		printk(KERN_ALERT "ERROR Requestig memory region 2 for GPIO driver?\n");

	gpio_remap_pc  = ioremap_nocache(0x40006048, 0x20);
	if (gpio_remap_pc == NULL)
		printk(KERN_ALERT "ERROR Remapping memory region 1 for GPIO driver?\n");
	gpio_remap_int = ioremap_nocache(0x40006100, 0x1c);
	if (gpio_remap_int == NULL)
		printk(KERN_ALERT "ERROR Remapping memory region 2 for GPIO driver?\n");
		
	//*GPIO_PC_MODEL = 0x33333333;
	gpio_remap_pc[1] = 0x33333333;/*
	gpio_remap_pc[4] = 0x33;
	gpio_remap_pc[5] = 0x33;
	gpio_remap_pc[6] = 0x33;
	gpio_remap_pc[7] = 0x33;*/

	//*GPIO_PC_DOUT = 0xff;
//	gpio_remap_pc[12] = 0xff;
	gpio_remap_pc[3] |= 0xff;

	// *GPIO_EXTIPSELL = 0x22222222;	// Choose port C pins for interrupt generation
	gpio_remap_int[0] = 0x22222222;/*
	gpio_remap_int[0] = 0x22;
	gpio_remap_int[1] = 0x22;
	gpio_remap_int[2] = 0x22;
	gpio_remap_int[3] = 0x22;*/
	
	// *GPIO_EXTIRISE = 0xff;			// gen int at rising edge
//	gpio_remap_int[8] = 0xff;
	gpio_remap_int[2] |= 0xff;
	// *GPIO_EXTIFALL = 0xff;			// gen int at falling edge
//	gpio_remap_int[12] = 0xff;
	gpio_remap_int[3] |= 0xff;
	// *GPIO_IEN = 0xff;				// Enable GPIO interrupts for 8 least significant bits
//	gpio_remap_int[16] = 0xff;
	gpio_remap_int[4] |= 0xff;


	error = alloc_chrdev_region(&dev, 0, 1, "gpDev");
	if (error < 0)
		printk(KERN_ALERT "ERROR Allocating character device for GPIO driver?\n");

	cdev_init(&my_cdev, &gamepad_fops);

	error = cdev_add(&my_cdev, dev, 1);
	if (error < 0)
		printk(KERN_ALERT "ERROR Adding character device for GPIO driver?\n");

	cl = class_create(THIS_MODULE, "gamepad");
	device_create(cl, NULL, dev, NULL, "gamepad");

	// Initially clearing interrupts
	gpio_remap_int[7] = gpio_remap_int[5];
	
	error = request_irq(17, gpio_handler, 0, "gpDev", 0);
	if (error < 0)
		printk(KERN_ALERT "ERROR requesting GPIO interrupt 17?\n");
		
	error = request_irq(18, gpio_handler, 0, "gpDev", 0);
	if (error < 0)
		printk(KERN_ALERT "ERROR requesting GPIO interrupt 18?\n");
		

	portC_din = 0;

	printk("Initialized GPIO driver\n");
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
	free_irq(17, 0);
	free_irq(18, 0);

	printk("Uninitialized GPIO driver\n");
}

// DRIVER FUNCTIONS

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
	//*buff = ~(gpio_remap_pc[7]);
	printk("R\n");
	*buff = portC_din;
	return 0;
}
static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	return 0;
}
/*
static int gamepad_ioctl(struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
{
	printk("Write works not? %i\n", cmd);
	//copy_from_user();
	//func_p = buff;
	return 0;
}*/

module_init(template_init);
module_exit(template_cleanup);
//module_init(__init);
//module_exit(__exit);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

