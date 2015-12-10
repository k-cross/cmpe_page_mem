# Networked Paging Kernel Module

##About
Kernel module and C++ application that can read and write paged memory from a host machine to a remote machine over a network connection

#####Implementation
This kernel module and C++ application complete the following actions to allow for pages in memory to mapped over the network:
- Userspace to kernel space communication
- System call hooking and redefinition (system call: PagFault)
- Two way communication and data transfer over a TCP socket

All code was written in C++ using additional librarys provided by Ubuntu 14 (Kernel and System Calls) and Boost (TCP sockets)

## Compile Instructions
* For compiling either the client or server, go to respective directory and type **make**.
* Tested for kernel versions 3.16 on LMDE2

## Network Instructions
For VirtualBox, go to **settings** in the host machine, click **network** and where there's the drop-down menu with _NAT_ as default, change it to **bridged**. This creates a network where you can see everything on your VMs network including your physical box. This makes having an internet connection easy to maintain.
* The school network doesn't allow normal VM bridged behavior 
    * Might bring a router or change network type specifically for presentation

## Commits/Code and Changes
* Client related code should stay client side 
* Server related code should stay server side 
* Try to put overlapping content into a separate file in the root project source folder
