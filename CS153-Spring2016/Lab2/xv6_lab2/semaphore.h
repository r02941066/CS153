#include "types.h"
#include "user.h"
#include "queue.h"

struct Semaphore
{
    lock_t locksem;
    int count;
    struct queue t_q;
};

void sem_init(struct Semaphore *s, int init_val);
void sem_acquire( struct Semaphore *s);
void sem_signal(struct Semaphore *s);
