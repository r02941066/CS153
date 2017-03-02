#include "types.h"
#include "user.h"

struct thread{
    int tid;
    int holding;
};
struct{
    lock_t lock;
    struct thread thread[64];
    int total;
}ttable;

struct {
    lock_t lock;
    int pass; 
    int holding_thread;
}frisbee;

static int numthreads = 0;
static int numpass = 0;
static int barrier = 0;
void pass_next(void *arg);
int lookup();



int main(int argc, char *argv[]){

    int i;
    struct thread *t;
//    void * sp;

    if(argc != 3){
        printf(1,"argc is not match !\n");
        exit();
    }
    numthreads = atoi(argv[1]);
    numpass = atoi(argv[2]);

    void * slist[numthreads];

    //init ttable;
    lock_init(&ttable.lock);
    ttable.total = 0;
    for(t=ttable.thread;t < &ttable.thread[64];t++){
        t->tid = 0;
    }
    //init stack list
    for(i = 0; i < 64;i++){
        slist[i]=0;
    }
    //init frisbee
    lock_init(&frisbee.lock);
    frisbee.pass = 0;
    frisbee.holding_thread = 0;

    printf(1,"\nnum of threads %d \n",numthreads);
    printf(1,"num of passes %d \n\n",numpass);


    for(i=0; i<numthreads;i++){
        void *stack = thread_create(pass_next,(void *)0);      
        if(stack == 0)
            printf(1,"thread_create fail\n");
        else{
            slist[i] = stack;
        }
    }
//    sleep(5);
    for(i=0;i<numthreads;i++){
        if(wait() == -1)
            break;
    }
   // add printf for tid look up.  
    for(t=ttable.thread;t < &ttable.thread[64];t++){
        if(t->tid != 0)
            printf(1,"thread %d was killed! stack was freed.\n",t->tid);
    }

    //free stacks
    for(i=0;i<numthreads;i++){
        if(slist[i] != 0){
            void * f = slist[i];
            free(f);
        }
    }
    exit();
}

void pass_next(void *arg){
    struct thread *t;
    int tid;

    tid = getpid();

    lock_acquire(&ttable.lock);
    for(t=ttable.thread;t < &ttable.thread[64];t++){
        if(t->tid == 0){
            t->tid = tid;
            break;
        } 
    }
    ttable.total++;
    lock_release(&ttable.lock);

   for(;;){
        lock_acquire(&ttable.lock);
        if(ttable.total == numthreads){
            printf(1," tid %d ready to go\n",t->tid);
            barrier++;
            goto start;
        }
        lock_release(&ttable.lock);
    }
    
//barriar above
start:
     lock_release(&ttable.lock);
     while(barrier != numthreads);
    //throw frisbee
    do{
        lock_acquire(&frisbee.lock);
        if(frisbee.pass > numpass){
            lock_release(&frisbee.lock);
            goto leaving;
        }
        if(frisbee.holding_thread == tid){
            lock_release(&frisbee.lock);
            sleep(5);
            continue;
        }
        printf(1,"pass: %d, thread %d catch the frisbee. throwing...\n",
                frisbee.pass, tid);
        frisbee.pass++;
        frisbee.holding_thread = tid;
        lock_release(&frisbee.lock);
    }while(1);

leaving: 
    lock_release(&frisbee.lock);
    printf(1,"thread %d out of game\n",tid);
    texit();
}

int lookup(int num_threads){
    int i;
    struct thread *t;
    i = 0;
    for(t=ttable.thread;t<&ttable.thread[64];t++){
        if(t->tid != 0){
            i++;
        }
    }
    return i;
}
