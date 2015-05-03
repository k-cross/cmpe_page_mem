/*
 * <Author> Kenneth Cross
 * <Date> 05/03/2015
 * <Matter> Kernel Module tests
 */

#include <linux/init.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */

MODULE_LICENSE("GPL"); /* Just include this, different licenses restrict usage */
MODULE_AUTHOR("CMPE142");
MODULE_DESCRIPTION("Test Code");

static int__init module_init(void){
    printk(KERN_INFO "Hello world 1.\n");

    return 0;
}

static void__exit module(void)
{
        printk(KERN_INFO "Goodbye world 1.\n");
}

module_init(module_init); /* Maybe just used to redefine names */
module_exit(module); 
