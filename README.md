# Linux-Strace

This is a final project for my Operating Systems class where we implemented the linux call "strace" in XV6. When run in conjunction with a command, strace  outputs all system calls made while the command is being executed. Strace can be turned on by typing the command "strace on" or off in a similar fashion. Typing "strace dump" will output the last 16 system calls made in the system. These system calls are stored in a circular buffer within the kernel, and can be accessed at any time to help with debugging. We included leak.c, which is a file that creates a memory leak within the kernel, to show how it can be useful in debugging. When leak.c is run, strace dump shows the last system calls made where sbrk() fails to distribute memory and the program exits. 

More detailed documentation and running instructions can be found in the attached PDF.
