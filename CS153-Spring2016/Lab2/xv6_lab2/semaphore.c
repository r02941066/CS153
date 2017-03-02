#include "semaphore.h"

void sem_init(struct Semaphore *s, int init_val)
{
  //s=malloc(sizeof(struct Semaphore));
  s->count=init_val;
  lock_init(&s->locksem);
  //s->t_q=malloc(sizeof(struct queue));
  init_q(&s->t_q);
  //printf(1,"Semaphore created\n");

}

void sem_acquire( struct Semaphore *s)
{
  lock_acquire(&s->locksem);
  //printf(1,"acquire lock acquired by: %d\n",getpid());

  if(s->count==0)
  {
    add_q(&s->t_q, getpid());
    lock_release(&s->locksem);
    //printf(1,"acquire ended release by %d\n",getpid());
    tsleep();
  }
  else
  {
    s->count--;
    lock_release(&s->locksem);
    if(s->count<0)
      s->count=0;
    lock_release(&s->locksem);
  }
}

void sem_signal(struct Semaphore *s)
{
  lock_acquire(&s->locksem);
  if(!empty_q(&s->t_q))
  {
      twakeup(pop_q(&s->t_q));
  }
  else
  {
    s->count++;
  }
  lock_release(&s->locksem);
}
