# Networked Paging
Kernel module that creates memory pages that are mapped to a networked computer.

**All non-project related code goes into the test folder, do not put our tests anywhere else, as the project code should remain in a logical directory structure.**

Use sockets, message queues, and semaphores. It might also be necessary to look into shared memory. The reference material came from the Linux Programming Interface.

VMs: LMDE2 is going to be used due to the increase in documentation and ease of use. Please setup a host virtual machine and then make clone (linked clone) with different uncloned MAC addresses for the client machine.

For VirtualBox, go to **settings** in the host machine, click **network** and where there's the dropdown menu with _NAT_ as default, change it to **bridged**. This creates a network where you can see everything on your VMs network including your physical box. This makes having an internet connection easy to maintain. 

* All client related code should stay in the client side 
* All server related code should stay server side 
    * There will be overlap, but let's develop the two separately for added flexibility.
