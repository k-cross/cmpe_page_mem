* Kernel Module that interfaces with Virtual Memory System and redirects page faults on a specific memory area of a process to a remote process via the network.

* Modify linux kernel so networked memory can be mapped to a given process. (hint: look at mmap() system call)

* idea is to store pages on the network, filling the TLB with new page pointers.

* when the process writes to the particular region, code can flush the new data across the network when sync is requested.

* hint: use user-level network API instead of kernel ones.


# Use the Client-Server Model approach:
* Communications between clients and servers is done by message passing.

* Interprocess Communication Notes: (Figure 43-1 from Linux Programming Interface provides a good structure) Focus on POSIX IPC facilities as System V are outdated.
* WILL ONLY USE POSIX IPCs will probably use Message based, not stream based.
    * Communication: Used mainly to exchange data
        * Data Transfer Facilities: One process writes data to an IPC facility, and another process reads the data. The facilities require two data transfers between user memory and kernel memory.
            * Byte Stream: data exchanged via pipes, FIFOs, and datagram sockets is an undelimited byte stream. Files are considered "sequences of bytes".
            * Message: Exchanged data by message queues and datagram sockets take the form of delimited messages
            * Pseudoterminals: Special uses
            * Synchronization between the reader and writer processes is automatic.
        * Shared Memory: Kernel accomplishes memory sharing by making page-table entries in each process point to the same pages of RAM.
            * Different from data-transfer as data-transfer is destructive when users try to read from it. It locks the file and consumes data.
            * Provides fast communication, but speed is offset by the need to synchronize ops
            * Data placed in shared memory is visible to all other processes in that share that memory.
    * Synchronization: Used to synchronize actions between processes and threads
        * Semaphores: Kernel-maintained integer whose value doesn't fall below 0. If decremented at 0, after it is incremented it will be decremented again. A good use for it is for evaluating the priority of a process or thread.
        * File locks: used to help coordinate the actions of multiple processes on a single file. There are 1) Read Locks and 2) Write Locks.
            * A read lock could happen if a file doesn't exist or no data is in the file, it will lock a read until data from something like a byte stream is added.
            * A write lock could happen when two processes are trying to write over the same area in memory.
        * Mutexes and Conditional Variables:
    * Signals: Tools for sync and sometimes communication.

* POSIX Specific IPC:
    * Message queues: used to pass messages between processes
        * Can be assigned priority
    * Semaphores: permit multiple processes to synchronize their actions. 
        * Allocated individually unlike System V's
        * Operated on individually

* Socket Notes: (For Networking)

* Memory Mapping Notes (For Page info)

# Will all C code work with C++?
Yes... kind of.
* C code might need to be written and accessed in the form of a library/header file.

# Things must happen atomically

Since getting pages from memory and putting them on the server must happen for both machines to work properly, there cannot be any duplication. So these must happen:

* Notification/verification that a page was received and put into memory for client/server
* Notification/verification that a page was deleted from memory for client/server
* For a event to officially occur and be finalized, all events in a log must happen, or none happen.
    * This provides protection from: loss of data and duplicated data

# Networking through POSIX

_Chapter 59 has good info_

* INADDR_LOOPBACK is C for loopback address
* INADDR_ANY is a wildcard address - used for internet domain sockets that bind on multihomed hosts.
* bind() is for binding a port to a socket
* /proc/sys/net/ipv4/ip_local_port_range may be the location of important net info
* AF_INET is programming with IPv4 sockets
* Networking with IPv4
    * #include <arpa/inet.h>
    * uint16_t htons(uint16_t host_uint16); //Returns host\_uint16 in network byte order
    * uint32_t htonl(^); //Returns same as above but 32 bit
    * uint32_t ntohl(uint32_t net_uint32); //Returns net\_uint32 in host byte order
    
