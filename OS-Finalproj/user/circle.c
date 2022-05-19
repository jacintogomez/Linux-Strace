// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user.h"

// #define BUFSIZE 4
// char buf[BUFSIZE];
// char *pIn, *pOut, *pEnd;
// char full;

// // init
// void buf_init()
// {
//     pIn = pOut = buf;       // init to any slot in buffer
//     pEnd = &buf[BUFSIZE];   // past last valid slot in buffer
//     full = 0;               // buffer is empty
// }

// // add char 'c' to buffer
// int buf_put(char c)
// {
//     if (pIn == pOut  &&  full)
//         return 0;           // buffer overrun

//     *pIn++ = c;             // insert c into buffer
//     if (pIn >= pEnd)        // end of circular buffer?
//         pIn = buf;          // wrap around

//     if (pIn == pOut)        // did we run into the output ptr?
//         full = 1;           // can't add any more data into buffer
//     return 1;               // all OK
// }

// // get a char from circular buffer
// char buf_get(char* pc)
// {
//     if (pIn == pOut  &&  !full)
//         return 0;           // buffer empty  FAIL

//     *pc = *pOut++;              // pick up next char to be returned
//     if (pOut >= pEnd)       // end of circular buffer?
//         pOut = buf;         // wrap around

//     full = 0;               // there is at least 1 slot
//     return 1;               // *pc has the data to be returned
// }

// void dump(char buf[BUFSIZE]){
//     for(int i=0;i<BUFSIZE;i++){
//         printf(1,"element %c\n",buf_get());
//     }
// }

// int main(int argc, char *argv[]) {
//     buf_init();
//     buf_put('j');
//     buf_put('t');
//     buf_put('d');
//     buf_put('m');
//     buf_put('o');
//     buf_put('h');
//     dump(buf);
//     exit();
// }