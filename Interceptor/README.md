# Intercepting Linux System Calls and Monitoring Processes

## Overview

The goal is to intercept system calls by writing and installing a very basic kernel module to the Linux kernel.

Here is what "intercepting a system call" means: You will implement a new system call named my_syscall, which will allow you to send commands from userspace, to intercept another pre-existing system call (like read, write, open, etc.). After a system call is intercepted, the intercepted system call would log a message first before continuing performing what it was supposed to do.

For example, if we call my_syscall with command REQUEST_SYSCALL_INTERCEPT and target system call number __NR_mkdir (which is the macro representing the system call mkdir) as parameters, then the mkdir system call would be intercepted; then, when another process calls mkdir, mkdir would log some message first, then perform what it was supposed to do (i.e. make a directory).

But actually we don't want mkdir to log a message whenever any process calls it. Instead, we only want mkdir to log a message when a certain set of processes (PIDs) are calling mkdir. In other words, we want to monitor a set of PIDs for the system call mkdir. Therefore, you will need to keep track, for each intercepted system call, of the list of monitored PIDs. Our new system call will support two additional commands to add/remove PIDs to/from the list.

When we want to stop intercepting a system call (let's say mkdir but it can be any of the previously intercepted system calls), we can invoke the interceptor (my_syscall), with a REQUEST_SYSCALL_RELEASE command as an argument and the system call number that we want to release. This will stop intercepting the target system call mkdir, and the behaviour of mkdir should go back to normal like nothing happened.


## Attention

Make sure you **run the interceptor on a virtual machine**, as it may potentially destroy your system (it has been fully tested, but just in case if anything went wrong).

[Ubuntu 10.04 VM download link from University of Toronto](http://www.cs.toronto.edu/~bogdan/stuff/UbuntuServer-CSC369-vmdk.tgz)

## Requirements

In order to be able to issue our own intercepting commands from userspace, we need a new system call that takes as parameters the command, the system call number (to be intercepted), and (for monitoring) a pid.

Instead of adding a new system call, which can be tricky, our new system call my_syscall will be installed in place of an unused system call in the system call table. We will connect my_syscall to the entry number MY_CUSTOM_SYSCALL (in effect, entry 0 which is mostly unused). The new system call my_syscall, defined as follows: int my_syscall(int cmd, int syscall, int pid); will serve as an interceptor and will receive the following commands from userspace:

* REQUEST_SYSCALL_INTERCEPT: intercept the system call syscall
* REQUEST_SYSCALL_RELEASE: de-intercept the system call syscall
* REQUEST_START_MONITORING: start monitoring process pid for system call syscall, i.e., add pid to the syscall's list of monitored PIDs. A special case is that if pid is 0 then all processes are monitored for syscall, but only root has the permission to issue this command (see the comments for my_syscall in the starter code for more details).
* REQUEST_STOP_MONITORING: stop monitoring process pid for system call syscall, i.e., remove pid from the syscall's list of monitored PIDs.


## Kernel module operation

Your kernel module must, upon initialization, replace the system call table entry for the MY_CUSTOM_SYSCALL number, with the my_syscall function. When the module is released, it must restore this system call to its original routine. As a result, when your kernel module is loaded, any subsequent invocations of the system call number MY_CUSTOM_SYSCALL from userspace, will issue four types of commands, to intercept or release a given system call, and to start and stop monitoring a pid for a specific syscall. You must implement the my_syscall function accordingly.

**1. REQUEST_SYSCALL_INTERCEPT and REQUEST_SYSCALL_RELEASE.**<br>
When an intercept command is issued, the corresponding entry in the system call table will be replaced with a generic interceptor function (discussed later) and the original system call will be saved. When a REQUEST_SYSCALL_RELEASE command is issued, the original saved system call is restored in the system call table in its corresponding position.

**2. REQUEST_START_MONITORING and REQUEST_STOP_MONITORING**<br>
Monitoring a process consists of the module logging into userspace some information about the process and the system call: the system call number, the parameters of the system call, and the pid of the process.

When a REQUEST_START_MONITORING command comes through our custom system call, the kernel module must record internally that the pid passed as a parameter should be monitored for the syscall number (passed as a parameter as well). The monitoring can be done for a specific pid, or for all pids (in which case the pid parameter for my_syscall will be 0).

We have also established that the generic interceptor function is what each intercepted system call will reach. In other words, whenever we reach the generic interceptor, we know that the system call is being intercepted (otherwise we would not reach this). If the pid of the process issuing the system call is being monitored, that means that we must print some information to a log. The log message will simply contain the system call number and the arguments, as well as the calling process's pid. 

As you might expect, regardless if a pid is monitored or not, the generic interceptor must eventually (once it's done logging, if applicable), call the original system call to allow normal operation of all processes in the system.

**What if a process exits before the user can issue a system call to stop monitoring it?**<br>
When your kernel module initializes, you should also intercept the exit_group system call (with number __NR_exit_group), by replacing it in the system call table with your own custom function my_exit_group. Of course, make sure to save the original exit_group function, and to restore it when your kernel module is unloaded. 


## Implementation details

**1.** Since the number of system calls is rather small (~300), and for performance reasons, you must maintain the system call information in an array. Each array element will contain information, as described in the mytable struct:

    typedef struct {
        asmlinkage long (*f)(struct pt_regs);
        int intercepted;
        int monitored;
        int listcount;
        struct list_head my_list;
    } mytable; 

**2.** You must use a linked list for storing information about the monitored processes; using an array of fixed size is entirely inadequate (because the search time will be the same as a linked list, the implementation complexity will be the same, but the size of the array will limit the number of entries).

**3.** The system call table is exported by the void* sys_call_table[NR_syscalls], present in one of the kernel source files from the VM image on Mathlab. If you wish to configure your own kernel image and re-compile it, you can modify the source code by adding the following two lines in the /usr/src/linux-source-2.6.32/arch/x86/kernel/i386_ksyms_32.c file:

    extern void* sys_call_table[];
    EXPORT_SYMBOL(sys_call_table);

then recompile the kernel. Again, our virtual machine image already has these changes in place.

**4.** Since the 2.6 kernel is preemptive, you must protect access to shared data. You will be using spinlocks for this purpose. The use of spinlocks is fairly simple and you have been shown some examples in one of the tutorials.

**5.** You must use the system call number 0 for MY_CUSTOM_SYSCALL. Do not attempt to use a different existing system call number, as that may result in the kernel misbehaving (to say the least). Remember that lots of services running in your OS make use of these system calls.

**6.** Logging the system call will be done using the log_message macro, defined in the interceptor.h header file.

**7.** For testing, you can use the provided tester programs. After you compile a test program (the provided Makefile only compiles your interceptor module, not any tester!), remember to run the tester using sudo privileges in the VM. 

To facilitate your testing, you should first try to implement the commands to intercept and release system calls. When you are ready to test these, use the test_intercept.c tester.
Once all tests pass, you can proceed to implementing the monitoring commands. To test all commands (both related to intercepting and to monitoring), you can use the test_full.c tester.


## Testing

To help you test your code, we have provided two testers, which you will also find in your repositories. To encourage you to test as you go, we are providing you with two testers:

* Tests if intercept and de-intercept commands work correctly.
	```c
    test_intercept.c
    ```

* Tests if all commands (including intercept, release, and both monitoring commands) work properly. This is a superset of test_intercept.c.
    ```c
    test_full.c 
	```

The tester loads your module and tests some basic functionality. It is by no means a comprehensive tool to ensure your code works for every corner case. To ensure that your code works correctly in all possible scenarios, you should add more test cases by modifying the testers (done). The tester will not catch synchronization bugs, except for blatant deadlocks. It is your responsibility to ensure that your code is not likely to run into synchonization problems.<br>
Finally, when testing, you will likely see the tester crash on various tests, due to bugs in your module. During your debugging, please feel free to go in each tester, and comment out some of the system calls being tested, if you wish to debug each test case in isolation.
