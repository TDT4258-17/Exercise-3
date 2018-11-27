#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>

//#include <linux/platform_device.h>
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


static struct file_operations gamepad_fops = 
{
	.owner = THIS_MODULE,
	.open = gamepad_open,
	.release = gamepad_release,
	.read = gamepad_read,
	.write = gamepad_write
};

irqreturn_t gpio_handler(int irq, void* dev_id)
{
	gpio_remap_int[7] = gpio_remap_int[5];
	portC_din = ~(gpio_remap_pc[7]);
	return 0;
}

static int __init gamepad_driver_init(void)
{

	//platform_driver_register(&my_driver);

	int error = 0;
	
	//struct resource* mem_resource = request_mem_region(0x40006048, 20, "CharDevice");
	request_mem_region(0x40006048, 0x24, "gpDev");
	request_mem_region(0x40006100, 0x20, "gpDev");

	gpio_remap_pc  = ioremap_nocache(0x40006048, 0x24);
	if (gpio_remap_pc == NULL)
	{
		printk(KERN_ALERT "ERROR Remapping memory region 1 for GPIO driver?\n");
		return -1;
	}
	gpio_remap_int = ioremap_nocache(0x40006100, 0x20);
	if (gpio_remap_int == NULL)
	{
		printk(KERN_ALERT "ERROR Remapping memory region 2 for GPIO driver?\n");
		return -1;
	}
		
	//*GPIO_PC_MODEL = 0x33333333;
	gpio_remap_pc[1] = 0x33333333;

	//*GPIO_PC_DOUT = 0xff;
	gpio_remap_pc[3] |= 0xff;

	// *GPIO_EXTIPSELL = 0x22222222;	// Choose port C pins for interrupt generation
	gpio_remap_int[0] = 0x22222222;
	
	// *GPIO_EXTIRISE = 0xff;			// gen int at rising edge
	gpio_remap_int[2] |= 0xff;

	// *GPIO_EXTIFALL = 0xff;			// gen int at falling edge
	gpio_remap_int[3] |= 0xff;

	// *GPIO_IEN = 0xff;				// Enable GPIO interrupts for 8 least significant bits
	gpio_remap_int[4] |= 0xff;


	error = alloc_chrdev_region(&dev, 0, 1, "gpDev");
	if (error < 0)
	{
		printk(KERN_ALERT "ERROR Allocating character device for GPIO driver?\n");
		return -1;
	}

	cdev_init(&my_cdev, &gamepad_fops);

	error = cdev_add(&my_cdev, dev, 1);
	if (error < 0)
	{
		printk(KERN_ALERT "ERROR Adding character device for GPIO driver?\n");
		return -1;
	}

	cl = class_create(THIS_MODULE, "gamepad");
	device_create(cl, NULL, dev, NULL, "gamepad");

	// Initially clearing interrupts
	gpio_remap_int[7] = gpio_remap_int[5];
	
	error = request_irq(17, gpio_handler, 0, "gpDev", 0);
	if (error < 0)
	{
		printk(KERN_ALERT "ERROR requesting GPIO interrupt 17?\n");
		return -1;
	}
		
	error = request_irq(18, gpio_handler, 0, "gpDev", 0);
	if (error < 0)
	{
		printk(KERN_ALERT "ERROR requesting GPIO interrupt 18?\n");
		return -1;
	}
		

	portC_din = 0;

	printk("Loaded GPIO driver\n");
	return 0;
}

static void __exit gamepad_driver_cleanup(void)
{
	gpio_remap_pc[1] = 0x0;
	gpio_remap_pc[3] = 0x0;
	gpio_remap_int[0] = 0x0;
	gpio_remap_int[2] = 0x0;
	gpio_remap_int[3] = 0x0;
	gpio_remap_int[4] = 0x0;

	free_irq(17, 0);
	free_irq(18, 0);
	device_destroy(cl, dev);
	class_destroy(cl);

	printk("Unloaded GPIO driver\n");
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
	*buff = portC_din;
	return 0;
}
static ssize_t gamepad_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{

	return 0;
}

module_init(gamepad_driver_init);
module_exit(gamepad_driver_cleanup);

MODULE_DESCRIPTION("Gamepad Module.");
MODULE_LICENSE("GPL");

