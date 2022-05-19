// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user.h"

// struct ringbuf{
//     int* buffer;
//     int head;
//     int tail;
//     int max;
//     int full;
// };

// void reset(struct ringbuf* me){
//     me->head=0;
//     me->tail=0;
//     me->full=0;
// }

// struct ringbuf* init(int* buffer,int size){
//     struct ringbuf* point=malloc(sizeof(struct ringbuf));
//     point->buffer=buffer;
//     point->max=size;
//     reset(point);
//     return point;
// }

// void freecb(struct ringbuf* me){free(me);}

// _Bool isfull(struct ringbuf* me){return me->full;}

// _Bool isempty(struct ringbuf* me){return (!me->full&&(me->head==me->tail));}

// int capacity(struct ringbuf* me){return me->max;}

// int size(struct ringbuf* me){
//     int size=me->max;
//     if(!me->full){
//         if(me->head>=me->tail){size=(me->head-me->tail);}
//         else{size=(me->max+me->head-me->tail);}
//     }
//     return size;
// }

// static void advancepointer(struct ringbuf* me){
//     // if(me->full){me->tail=(me->tail+1)%me->max;}
//     // me->head=(me->head+1)%me->max;
//     // me->full=(me->head==me->tail);
//     if(me->full){
//         if(++(me->tail)==me->max){me->tail=0;}
//     }
//     if(++(me->head)==me->max){me->head=0;}
//     me->full=(me->head==me->tail);
// }

// static void retreatpointer(struct ringbuf* me){
//     me->full=0;
//     if(++(me->tail)==me->max){me->tail=0;}
// }

// void insert(struct ringbuf* me,char data[]){
//     me->buffer[me->head]=data;
//     advancepointer(me);
// }

// void takeout(struct ringbuf* me,int* data){
//     if(!isempty(me)){
//         *data=me->buffer[me->tail];
//         retreatpointer(me);
//     }else{
//         printf(1,"error: attempt to remove data from empty buffer\n");    
//         printf(1,"exiting program\n");
//         exit();
//     }
// }

// void dump(struct ringbuf* me,int size){
//     for(int i=0;i<size;i++){
//         printf(1,"element is %c\n",me->buffer[i]);
//     }
// }

// int main(int argc, char *argv[]) {
//     int* cb=malloc(4*50*sizeof(char));
//     struct ringbuf* me=init(cb,4);
//     insert(me,"jacinto");
//     insert(me,"gomez");
//     insert(me,"in");
//     insert(me,"here");
//     insert(me,"zek");
//     dump(me,4);
//     // _Bool full=isfull(me);
//     // _Bool empty=isempty(me);
//     free(cb);
//     exit();
// }