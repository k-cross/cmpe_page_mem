/* 
 * <AUTHOR> Kennth Cross
 * <DATE>   05/05/2015
 * <MATTER> Stiching Mason's and Ziyi's modules into one
 * coherent final module that does it all!!!
 *
 * Giving up on shitty fucking Ziyi's code. Document your fucking shit you cunt
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/highmem.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/mm.h>
#include <asm/unistd.h>
#include <asm/page.h>
#include <net/sock.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CMPE142 Group");
MODULE_DESCRIPTION("Remote Memory module for the CMPE 142 Project");

/* Netlink Defined Things */

#define NETLINK_USER 31

struct sock *nl_sk = NULL; /* WTF do these values mean, these names suck assholes! */

static void hello_nl_recv_msg(struct sk_buff *skb) {
  struct nlmsghdr *nlh;
  int pid;
  struct sk_buff *skb_out;
  int msg_size;
  char *msg="Hello from kernel";
  int res;
  
  printk(KERN_INFO "Entering: %s\n", __FUNCTION__);
  
  msg_size=strlen(msg);
  
  nlh=(struct nlmsghdr*)skb->data;
  printk(KERN_INFO "Netlink received msg payload:%s\n",
          (char*)nlmsg_data(nlh));
  pid = nlh->nlmsg_pid; //pid of sending process 
  
  skb_out = nlmsg_new(msg_size,0);
  
  if(!skb_out){
      printk(KERN_ERR "Failed to allocate new skb\n");
      return;
  } 

  nlh = nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  
  NETLINK_CB(skb_out).dst_group = 0; //not in mcast group
  strncpy(nlmsg_data(nlh),msg,msg_size);
  res = nlmsg_unicast(nl_sk,skb_out,pid);

  if(res<hello_nl_recv_msg)
      printk(KERN_INFO "Error while sending bak to user\n");
}

/* Memory Defined Things */

/* System Call Table address found with:
grep "sys_call_table" /boot/System.map-3.13.0-51-generic */

unsigned long *sys_call_table = (unsigned long*)0xffffffff81801400; //NOTE! THIS IS SYSTEM SPECIFIC

/* 0: real System call is in use
   1: custom system call is in use */

int is_set=0;

/* We will set this variable to 1 in our open handler and erset it
back to zero in release handler */
int in_use = 0;

/* IOCTL commands -- we have defined these values only for us */
#define IOCTL_PATCH_TABLE 0x00000001
#define IOCTL_FIX_TABLE   0x00000004
void *r_addr, *r_addr_end;

// Pointer to original sys_mmap system function
asmlinkage long (*real_mmap)(unsigned long, size_t, 
        unsigned long, unsigned long, unsigned long, unsigned long);

// Replacement sys_mmap
asmlinkage long custom_mmap (unsigned long addr, size_t len, 
        unsigned long prot, unsigned long flags, unsigned long fd, unsigned long offset)
{
	printk("interceptor: open(%zx, %zx, %lu)\n", addr, len, prot);
	if((void*)addr == r_addr){
    printk("Intercepted Page Fault from RM_client at: 0x%zx of length: %zx\n", addr, len);
    printk("Redirecting to RM_server...");

		// TODO: Write back the Value to client app

		return 0;
	}
	// Otherwise, pass mmap as normal
	return real_mmap(addr, len, prot, flags, fd, offset);
}

// This part is to allow us to edit the sys_call_table directly (even though it is protected)
/* Make the page writable*/
int make_rw(unsigned long address){
	unsigned int level;
	// pte_t is Page Table Entry
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
static int our_ioctl(struct file *file, unsigned int cmd, 
        void* start_a, void* end_a, unsigned long arg)
{
	int retval = 0;

	r_addr = start_a;
	r_addr_end = end_a;

	//Implementation of iocrl - To patch __NR_mmap (sys_mmap) in the sys_call_table
	switch(cmd)
  {
	case IOCTL_PATCH_TABLE:
		make_rw((unsigned long)sys_call_table); // make table writeable
		real_mmap = (void*)*(sys_call_table + __NR_mmap); // store the table's  sys_mmap pointer
		*(sys_call_table + __NR_mmap) = (unsigned long)custom_mmap; // write custom_mmap pointer
		make_ro((unsigned long)sys_call_table); // make table protected
		is_set=1; // custom function written!
		break;
	case IOCTL_FIX_TABLE:
		make_rw((unsigned long)sys_call_table);
		*(sys_call_table + __NR_mmap) = (unsigned long)real_mmap; // write the stored real_mmap
		make_ro((unsigned long)sys_call_table);
		is_set=0; // original function restored!
		break;
	default:
		printk("Writing values to the sys_call_table has failed!\n");
		break;
	}

	return retval;
}

static const struct file_operations our_fops =\
{
  .owner = THIS_MODULE,
  .open = &our_open,
  .release = &our_release,
  .unlocked_ioctl = (void*)&our_ioctl,
  .compat_ioctl = (void*)&our_ioctl
};
static struct miscdevice our_device = \
{
  MISC_DYNAMIC_MINOR,
  "interceptor",
  &our_fops
};

static int __init init_mod(void){
	int retval = misc_register(&our_device); // Register device with system

	printk(KERN_INFO "Remote memory Module Successfully Initialized!\n");
	return retval; // Note: Non-zero return means module couldn't be loaded.

  /* Netlink's main functions */
  printk("Entering: %s\n",__FUNCTION__);
  
  struct netlink_kernel_cfg cfg = {
      .input = hello_nl_recv_msg,
  };

  nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
  
  if(!nl_sk){
      printk(KERN_ALERT "Error creating socket.\n");
      return -10;
  }
  
  return 0;
}

static void __exit cleanup_mod(void)
{
	misc_deregister(&our_device); //Unregister this device with the system

	if(is_set){
		make_rw((unsigned long)sys_call_table);
		*(sys_call_table + __NR_mmap) = (unsigned long)real_mmap;
		make_ro((unsigned long)sys_call_table);
	}

	printk(KERN_INFO "Remote Memory Module Is cleaning up and Exiting...\n");

  /* Netlink Cleanup */
  printk(KERN_INFO "exiting hello module\n");
  netlink_kernel_release(nl_sk);
}

module_init(init_mod);
module_exit(cleanup_mod);
