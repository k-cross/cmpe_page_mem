#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/miscdevice.h> // Included for the character driver
#include <linux/fs.h>
#include <linux/highmem.h>
#include <asm/unistd.h>
#include <asm/page.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mason Itkin");
MODULE_DESCRIPTION("Remote Memory module for the CMPE 142 Project");

/* System Call Table address found with:
grep "sys_call_table" /boot/System.map-3.13.0-51-generic */

unsigned long *sys_call_table = (unsigned long*)0xffffffff81801400; //NOTE! THIS IS SYSTEM SPECIFIC

// 0: real System call is in use
// 1: custom system call is in use
int is_set=0;

/* We will set this variable to 1 in our open handler and erset it
back to zero in release handler*/
 int in_use = 0;

// IOCTL commands -- change offset of all calls?
   #define IOCTL_PATCH_TABLE 0x00000001
   #define IOCTL_FIX_TABLE   0x00000004

/* Pointer to the original sys_open */
asmlinkage int (*real_open)(const char* __user, int, int);

/* Our replacement */
asmlinkage int custom_open(const char* __user file_name, int flags, int mode){
   printk("interceptor: open(\"%s\", %X, %X)\n", file_name,
                                                 flags,
                                                 mode);
   return real_open(file_name, flags, mode);
}

/* This part is to allow us to edit the sys_call_table directly (even though it is protected)
	Make the page writable */
int make_rw(unsigned long address){
	unsigned int level;
	/* pte_t --> Page Table Entry */
	pte_t *pte = lookup_address(address, &level);
	if(pte->pte &~ _PAGE_RW)
		pte->pte |= _PAGE_RW;
	return 0;
   }
   /* Make the page write protected */
int make_ro(unsigned long address){
	unsigned int level;
	pte_t *pte = lookup_address(address, &level);
	pte->pte = pte->pte &~ _PAGE_RW;
	return 0;
   }

 /* This function will be invoked each time a user process attempts
     to open our device. You should keep in mind that the prototype
    of this function may change along different kernel versions. */
static int our_open(struct inode *inode, struct file *file){
    /* We would not like to allow multiple processes to open this device */
    if(in_use)
       return -EBUSY;
    in_use++;
    printk("device has been opened\n");
    return 0;
}

 /* This function, in turn, will be called when a process closes our device */
 static int our_release(struct inode *inode, struct file *file){
    in_use--;
    printk("device has been closed\n");
    return 0;
 }

 /* This function will handle ioctl calls performed on our device */
 static int our_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    int retval = 0;

    switch(cmd){ /*Ioctl Implementation */
          case IOCTL_PATCH_TABLE:
             make_rw((unsigned long)sys_call_table);
             real_open = (void*)*(sys_call_table + __NR_open);
             *(sys_call_table + __NR_open) = (unsigned long)custom_open;
             make_ro((unsigned long)sys_call_table);
             is_set=1;
             break;
          case IOCTL_FIX_TABLE:
             make_rw((unsigned long)sys_call_table);
             *(sys_call_table + __NR_open) = (unsigned long)real_open;
             make_ro((unsigned long)sys_call_table);
             is_set=0;
             break;
          default:
             printk("Ooops....\n");
             break;
       }

    return retval;
 }

 static const struct file_operations our_fops =\{
       .owner = THIS_MODULE,
       .open = &our_open,
       .release = &our_release,
       .unlocked_ioctl = (void*)&our_ioctl,
       .compat_ioctl = (void*)&our_ioctl
    };

 static struct miscdevice our_device = \{
       MISC_DYNAMIC_MINOR,
       "interceptor",
       &our_fops
    };

static int __init init_mod(void){
	int retval = misc_register(&our_device); // Register this device with the system

    printk(KERN_INFO "Remote memory Module Successfully Initialized!\n");
    return retval;    // Note: Non-zero return means that the module couldn't be loaded.
}

static void __exit cleanup_mod(void){
	misc_deregister(&our_device); //Unregister this device with the system

	// check to see if our system call is in use and if it it revert back before exiting (rmmod)
	if(is_set){
	      make_rw((unsigned long)sys_call_table);
	      *(sys_call_table + __NR_open) = (unsigned long)real_open;
	      make_ro((unsigned long)sys_call_table);
	   }

    printk(KERN_INFO "Remote Memory Module Is cleaning up and Exiting...\n");
}

module_init(init_mod);
module_exit(cleanup_mod);