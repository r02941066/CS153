#include "types.h"
#include "user.h"

struct thread{
    int tid;
};

struct{
    lock_t lock;
    struct thread threads[64];
    int total;
}ttable;

void func(void *arg_ptr);

int main(int argc, char *argv[]){
    struct thread * t;
    int i;
    printf(1,"init ttable\n");
    lock_init(&ttable.lock);
    ttable.total = 0;

    lock_acquire(&ttable.lock);
    for(t=ttable.threads;t < &ttable.threads[64];t++){
        t->tid = 0;
    }
    lock_release(&ttable.lock);
    printf(1,"testing thread sleep and wakeup \n\n\n");
    void *stack = thread_create(func,0);
    thread_create(func,0);
    thread_create(func,0);

    i=0;
    while(i++ < 1000000);
    //find that thread
    lock_acquire(&ttable.lock);
    for(t=ttable.threads;t < &ttable.threads[64];t++){
        if(t->tid != 0){
            printf(1,"found one... %d,   wake up lazy boy !!!\n",t->tid);
            twakeup(t->tid);
            i++;
        }
    }
    lock_release(&ttable.lock);
    wait();
    wait();
    wait();
    free(stack);
    exit();
}

void func(void *arg_ptr){
    int tid;
    tid = getpid();
    lock_acquire(&ttable.lock);
    (ttable.threads[ttable.total]).tid = tid;
    ttable.total++;
    lock_release(&ttable.lock);

    printf(1,"I am thread %d, is about to sleep\n",tid);
    tsleep();
    printf(1,"I am wake up!\n");
    texit();
}
