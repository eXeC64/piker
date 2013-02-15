#Piker
Monolithic Kernel for the Raspberry Pi

##About
This is a kernel I've been working on in some of my spare time for fun.
My hope is to create a basic plan9-like operating system.

The name itself is just a portmanteau of "Pi" and "Kernel". I'm studying computer science after all, not creative writing.

Harry.

##Todo list

* Finish memory management
    * Implement dynamic memory allocation in kernel space
    * Implement paging for user space
* Start handling interrupts
    * Add some basic syscalls
* Run some code in userspace
    * Add contexts
    * Add scheduling
* Implement VFS
    * Load initramfs
    * Interface for virtual files for drivers
* Add some drivers
    * SD driver
    * FAT driver
    * uart driver
* Become self hosted
    * Port newlib/libc
    * Run userspace programs

##Design

###Philosophy
The design philosophy is based off of unix, and some of plan9.
The core principle is that everything is a file or file system, and that is the universal interface.


###Memory Map

####Virtual Memory

    --------------------- USERSPACE
    0x00000000-0x3FFFFFFF paged for userspace
    --------------------- USERSPACE

    --------------------- KERNELSPACE
    0x40000000-0xBFFFFFFF unmapped
    0xC0000000-0xDFFFFFFF raw memory mapping
    0xE0000000-0xF1FFFFFF unmapped
    0xF2000000-0xF2FFFFFF raw peripheral mapping
    0xF3000000-0xFFFFFFFF unmapped
    --------------------- KERNELSPACE

####Physical Memory

    0x00000000-0x1FFFFFFF raw memory
    0x20000000-0x20FFFFFF io memory

