# Networked Paging
Kernel module that creates memory pages that are mapped to a networked computer.

* For compiling either the client or server, go to respective directory and type **make**.

For VirtualBox, go to **settings** in the host machine, click **network** and where there's the drop-down menu with _NAT_ as default, change it to **bridged**. This creates a network where you can see everything on your VMs network including your physical box. This makes having an internet connection easy to maintain.
* The school network doesn't allow normal VM bridged behavior 
    * Might bring a router or change network type specifically for presentation

* Client related code should stay client side 
* Server related code should stay server side 
    * Try to put overlapping content into a separate file in the root project source folder
