* Kernel Module that interfaces with Virtual Memory System and redirects page
faults on a specific memory area of a process to a remote process via the
network.

* Modify linux kernel so networked memory can be mapped to a given process.
(hint: look at mmap() system call)

* idea is to store pages on the network, filling the tlb with new page
pointers.

* when the process writes to the particular region, code can flush the new data
across the network when sync is requested.

* hint: use user-level network API instead of kernel ones.


# Use the Client-Server Model approach:
* Communications between clients and servers is done by message passing.

* Interprocess Communication Notes: (Figrue 43-1 from Linux Programming
Interface provides a good structure) Focus on POSIX IPC facilities as System V
are outdated.
* WILL ONLY USE POSIX IPCs will probably use Message based, not stream based.
    * Communication: Used mainly to exchange data
        * Data Transfer Facilities: One process writes data to an IPC facility,
        and another process reads the data. The facilities require two data
        transfers between user memory and kernel memory.
            * Byte Stream: data exchanged via pipes, FIFOs, and datagram
            sockets is an undelimited byte stream. Files are considered
            "sequences of bytes".
            * Message: Exchanged data by message queues and datagram sockets
            take the form of delimited messages
            * Pseudoterminals: Special uses
            * Synchronization between the reader and writer processes is
            automatic.
        * Shared Memory: Kernel accomplishes memory sharing by making
        page-table entries in each process point to the same pages of RAM.
            * Different from data-transfer as data-transfer is destructive when
            users try to read from it. It locks the file and consumes data.
            * Provides fast communication, but speed is offset by the need to
            synchronize ops. 
            * Data placed in shared memory is visible to all other processes in
            that share that memory.
    * Synchronization: Used to synchronize actions between processes and
    threads
        * Semaphores: Kernel-maintained integer whose value doesn't fall below
        0. If decremented at 0, after it is incremented it will be decremented
        again. A good use for it is for evaluating the priority of a process
        or thread.
        * File locks: used to help coordinate the actions of multiple processes
        on a single file. There are 1) Read Locks and 2) Write Locks.
            * A read lock could happen if a file doesn't exist or no data is in
            the file, it will lock a read until data from something like a
            byte stream is added.
            * A write lock could happen when two processes are trying to write
            over the same area in memory.
        * Mutexes and Conditional Variables:
    * Signals: Tools for sync. and sometimes communication.

* POSIX Specific IPC:
    * Message queues: used to pass messages between processes
        * Can be assigned priority
    * Semaphores: permit multiple processes to synchronize their actions. 
        * Allocated individually unlike System V's
        * Operated on individually

* Socket Notes: (For Networking)

* Memory Mapping Notes (For Page info)

# Will all C code work with C++?

# Things must happen atomically
