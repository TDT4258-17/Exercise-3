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
	 printk("Unladed Sound driver\n");
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Sound module.");
MODULE_LICENSE("GPL");

