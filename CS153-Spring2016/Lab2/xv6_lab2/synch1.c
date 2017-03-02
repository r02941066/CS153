//#include "types.h"
//#include "user.h"
#include "semaphore.h"

void O_ready(void * arg_ptr);
void H_ready(void * arg_ptr);

struct Semaphore s_h;
struct Semaphore s_o;

struct mutex
{
	lock_t lock;
} mutex;

int hmol,omol;
int count = 0;

int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		printf(1,"wrong number of arguments\n");
	}

	hmol=atoi(argv[1]);
	omol=atoi(argv[2]);
	printf(1,"H-atoms created: %d \n",hmol);
	printf(1,"O-atoms created: %d \n",omol);

	sem_init(&s_h, 0);
	sem_init(&s_o, 0);
	lock_init(&mutex.lock);

	printf(1, "start ...\n");
	int i=0;
	for (i=0;i<hmol;i++)
	{
		void * tid2 = thread_create(H_ready, (void*) 0);
		if(tid2 == 0)
		{
			printf(1, "thread_create failed.\n");
			exit();
		}
		else
		{
			printf(1, "made one H atom!\n");
		}
	}
	for(i=0;i<omol;i++)
	{
		void * tid3 = thread_create(O_ready, (void*) 0);
		if(tid3 == 0)
		{
			printf(1, "thread_create failed.\n");
			exit();
		}
		else
		{
			printf(1, "made one O atom!\n");
		}
	}

	while(wait()>0);
	printf(1, "count = %d\n", count);

	exit();
	return 0;
}

void O_ready(void * arg_ptr)
{


	sem_acquire(&s_h);
	sem_acquire(&s_h);
	sem_signal(&s_o);
	sem_signal(&s_o);

	lock_acquire(&mutex.lock);
	count++;
	printf(1, "made one water molecule!\n");
	lock_release(&mutex.lock);

	texit();
}

void H_ready(void * arg_ptr)
{
	sem_signal(&s_h);
	sem_acquire(&s_o);


	texit();
}
