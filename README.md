# Networked Paging
Kernel module that creates memory pages that are mapped to a networked computer.

Please use [GNU Coding Conventions](https://gcc.gnu.org/codingconventions.html)
for C that way we don't have to argue.

**All non-project related code goes into the test folder, do not put our tests anywhere else, as the project code should remain in a logical directory structure.**

Use sockets, message queues, and semaphores. It might also be necesarry to look into shared memory. The reference material came from the Linux Programming Interface.

VMs: Using the Fedora21 Workstation ISO - Please install and setup the network.

For VirtualBox, on your host machine open a console and run this command _VBoxManage dhcpserver add --netname testlab --ip 10.10.10.1 --netmask 255.255.255.0 --lowerip 10.10.10.2 --upperip 10.10.10.12 --enable_ Next open the Vbox gui and click on settings for each VM then go to the network tab and choose "internal network". Change "intnet" to "testlab".

All client related code should stay in the client side and all server related code should stay server side there will be overlap, but let's develop the two seperately for added flexibility.
