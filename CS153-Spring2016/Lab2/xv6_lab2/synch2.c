#include "semaphore.h"

void monkey_climb();
void dominant_monkey_climb();

struct Semaphore tree, dominant_monkey;
lock_t mutex;
int main(int argc, char *argv[])
{
  //Monkeys climb the tree six times to demonstrate the phenomenom of
  //dominant monkeys.
  int n_m=atoi(argv[1]);
  int d_m=atoi(argv[2]);
  printf(1,"normal monkeys created: %d\n",n_m);
  printf(1,"dominant monkeys created: %d\n",d_m);
  sem_init(&tree,3);
  if(d_m==0)
  sem_init(&dominant_monkey,1);
  else
  sem_init(&dominant_monkey, d_m);
  lock_init(&mutex);
  int j=0;
  if(argc!=3)
  {
    printf(1,"invalid number of arguments\n");
    exit();
  }
  void *norm_monkey;
  int i;
  for(i=0;i<n_m;i++)
  {
    norm_monkey=thread_create(monkey_climb, (void*)&j);
    if(norm_monkey==0)
    {
      printf(1,"thread create fail\n");
      exit();
    }
  }

  void *dom_monkey;
  for(i=0;i<d_m;i++)
  {
    dom_monkey=thread_create(dominant_monkey_climb, (void*)&j);
    if(dom_monkey==0)
    {
      printf(1,"thread create fail\n");
      exit();
    }
    //printf(1,"dominant monkey thread created\n");
    sem_acquire(&dominant_monkey);
  }
  while(wait()>0);
  printf(1,"Oprah: coconuts for everybody!\n");
  exit();

}

void monkey_climb()
{
  sem_acquire(&dominant_monkey);
  sem_acquire(&tree);
 printf(1,"monkey %d gets coconut\n", getpid());
sem_signal(&tree);
  sem_signal(&dominant_monkey);
  texit();
}

void dominant_monkey_climb()
{
  sem_acquire(&tree);
printf(1,"dominant Monkey %d gets coconut\n", getpid());
sem_signal(&tree);
  sem_signal(&dominant_monkey);
  texit();
}
