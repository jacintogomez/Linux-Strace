#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/strace.h" //Trace program file
#include "user.h"



void procfork()
{
    int frtest = fork();
    if (frtest == -1) // failed to fork
    {
        printf(1, "Forking failed!\n");
        return;
    }
    else if (frtest == 0){
        close(open("README", 0)); // Leave child
        exit();
    }
    else 
    {
        wait();
    }

}

int main() {
    printf(1, "The process is now being traced!\n");
    
    strace(straceOn);
    
    procfork();

    strace(straceOff);
    printf(1, "Process and its forked processes are now being traced!\n");
    strace(straceOn | straceFork);
    procfork();

    strace(straceOff);
    printf(1, "Process no longer being traced!\n");
    procfork();

    exit();
}







