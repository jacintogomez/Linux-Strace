#include "kernel/types.h"
#include "kernel/defs.h"
#include "kernel/param.h"
#include "kernel/memlayout.h"
#include "kernel/mmu.h"
#include "kernel/proc.h"
#include "kernel/x86.h"
#include "kernel/syscall.h"
#include "kernel/strace.h"

// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.


#define N 16
int place=0;
int track=1;

struct ring{
    int pid;
    char* name;
    char* syscall;
    int retval;
};

static struct ring buf[N];

// static struct ring pretty[1000];
// int pcount=0;
// int ptrack=1;
// void addtopret(int id,char* nm,char* sc,int rv){
//     if(pcount>=1000){pcount=0;}
//     pretty[place].pid=id;
//     pretty[place].name=nm;
//     pretty[place].syscall=sc;
//     pretty[place].retval=rv;
//     pcount++;
//     if(track>=N-1){track=0;}
//     else{track++;}
// }
// void readable(){
//     //track--;
//     for(int u=0;u<1000;u++){
//         if(ptrack>=1000){ptrack=0;}
//         cprintf("\e[35mTRACE: pid = %d | process name = %s | syscall = %s | return val = %d\e[0m\n", pretty[ptrack].pid, pretty[ptrack].name, pretty[ptrack].syscall,pretty[ptrack].retval);
//         ptrack++;
//     }
// }

void addtobuf(int id,char* nm,char* sc,int rv){
    if(place>=N){place=0;}
    buf[place].pid=id;
    buf[place].name=nm;
    buf[place].syscall=sc;
    buf[place].retval=rv;
    place++;
    if(track>=N-1){track=0;}
    else{track++;}
}

void printout(){
    //track--;
    for(int u=0;u<N;u++){
        if(track>=N){track=0;}
        cprintf("\e[35mTRACE: pid = %d | process name = %s | syscall = %s | return val = %d\e[0m\n", buf[track].pid, buf[track].name, buf[track].syscall,buf[track].retval);
        track++;
    }
}

// Fetch the int at addr from the current process.
int fetchint(uint addr, int *ip) {
  if (addr >= proc->sz || addr + 4 > proc->sz)
    return -1;
  *ip = *(int *)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int fetchstr(uint addr, char **pp) {
  char *s, *ep;

  if (addr >= proc->sz)
    return -1;
  *pp = (char *)addr;
  ep = (char *)proc->sz;
  for (s = *pp; s < ep; s++)
    if (*s == 0)
      return s - *pp;
  return -1;
}

// Fetch the nth 32-bit system call argument.
int argint(int n, int *ip) { return fetchint(proc->tf->esp + 4 + 4 * n, ip); }

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size n bytes.  Check that the pointer
// lies within the process address space.
int argptr(int n, char **pp, int size) {
  int i;

  if (argint(n, &i) < 0)
    return -1;
  if ((uint)i >= proc->sz || (uint)i + size > proc->sz)
    return -1;
  *pp = (char *)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int argstr(int n, char **pp) {
  int addr;
  if (argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_stracedump(void);

int sys_strace() {
    int x;
    // int* cb=0;
    // struct ringbuf* me=init(cb,16); 
    argint(0, &x); // we will use this function to grab parameters from user passed parameters
    struct proc* curproc = proc;
    curproc->traceflag = (x & straceOn) ? x : 0;
    return 0;
}

int sys_stracedump() {
    printout();
    return 0;
}

static int (*syscalls[])(void) = {
    [SYS_fork] sys_fork,   [SYS_exit] sys_exit,     [SYS_wait] sys_wait,
    [SYS_pipe] sys_pipe,   [SYS_read] sys_read,     [SYS_kill] sys_kill,
    [SYS_exec] sys_exec,   [SYS_fstat] sys_fstat,   [SYS_chdir] sys_chdir,
    [SYS_dup] sys_dup,     [SYS_getpid] sys_getpid, [SYS_sbrk] sys_sbrk,
    [SYS_sleep] sys_sleep, [SYS_uptime] sys_uptime, [SYS_open] sys_open,
    [SYS_write] sys_write, [SYS_mknod] sys_mknod,   [SYS_unlink] sys_unlink,
    [SYS_link] sys_link,   [SYS_mkdir] sys_mkdir,   [SYS_close] sys_close,
    [SYS_strace] sys_strace, [SYS_stracedump] sys_stracedump,
};

//Adding an array of the actual names of the system calls so that we can print out names instead of numbers
static char *syscall_names[] = {
[SYS_fork]    "fork", [SYS_exit]    "exit", [SYS_wait]    "wait",
[SYS_pipe]    "pipe", [SYS_read]    "read", [SYS_kill]    "kill",
[SYS_exec]    "exec", [SYS_fstat]   "fstat", [SYS_chdir]   "chdir",
[SYS_dup]     "dup", [SYS_getpid]  "getpid", [SYS_sbrk]    "sbrk",
[SYS_sleep]   "sleep", [SYS_uptime]  "uptime", [SYS_open]    "open",
[SYS_write]   "write", [SYS_mknod]   "mknod", [SYS_unlink]  "unlink",
[SYS_link]    "link", [SYS_mkdir]   "mkdir", [SYS_close]   "close",
[SYS_strace]   "strace", [SYS_stracedump] "stracedump",
};

void syscall(void) {
  int num, j;

  struct proc* currentprocess = proc;
  int isTraced = (currentprocess->traceflag & straceOn);
  char processName[16];

  for (j = 0; currentprocess->name[j] != 0; j++) // We copy over the process name
  {
      processName[j] = currentprocess->name[j];
  }
  processName[j] = currentprocess->name[j];

  num = currentprocess->tf->eax;
    addtobuf(currentprocess->pid,processName,syscall_names[num],currentprocess->tf->eax);
    //addtopret(currentprocess->pid,processName,syscall_names[num],currentprocess->tf->eax);

    //char* exit="exit";
    //cprintf("process name %s\n",processName);
    // if(syscall_names[num]==exit){
    //     printout();
    // }
   if(num == SYS_exit && isTraced) 
   {
       //readable();
       cprintf("\e[35mTRACE: pid = %d | process name = %s | syscall = %s\e[0m\n", currentprocess->pid, processName, syscall_names[num]);
   }

  if (num > 0 && num < NELEM(syscalls) && syscalls[num]) 
  {
      proc->tf->eax = syscalls[num]();
      if (isTraced) // Colouring the output to differentiate it from regular output. Successful exit system calls are discarded.
      {     
          cprintf((num == SYS_exec && currentprocess->tf->eax == 0) ? "\e[35mTRACE: pid = %d | process name = %s | syscall = %s\e[0m\n" :"\e[35mTRACE: pid = %d | process name = %s | syscall = %s | return val = %d\e[0m\n", currentprocess->pid, processName, syscall_names[num], currentprocess->tf->eax);
          //addtobuf(currentprocess->pid,processName,syscall_names[num],currentprocess->tf->eax);
        }
  } else {
      //addtobuf(proc->pid,proc->name,"unknown sys call",-1);
    cprintf("%d %s: unknown sys call %d\n", proc->pid, proc->name, num);
    proc->tf->eax = -1;
  }
  
}
 