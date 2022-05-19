#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int count=0;
int memused=0;

void leak(){
    int* waste=(int*)malloc(5000*sizeof(int));
    if(!waste){
        //printf(1,"All memory used, terminating program\n");
        memused+=1;
    }
    if(memused){
        exit();
    }
    count+=*waste;
    return;
}

int main(int argc, char *argv[]) {
    for(;;){leak();}
    exit();
}