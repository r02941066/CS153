#include "semaphore.h"

int numM=0;
int numC=0;

lock_t mutex;
struct Semaphore boat;

void missionaryArrives(void* arg);
void cannibalArrives(void* arg);
void rowboat();
int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf(1,"invalid number of arguments\n");
		exit();
	}
	lock_init(&mutex);
	sem_init(&boat,3);
	void* temp;
	int i=0;
	lock_acquire(&mutex);
	for(;i<atoi(argv[1]);i++)
	{
		if(random(2600)%2==0)
			temp=thread_create(missionaryArrives,&i);
		else
			temp=thread_create(cannibalArrives,&i);
		if(temp==0)
		{
			printf(1,"thread create failed\n");
			exit();
		}
	}
	lock_release(&mutex);
	while(wait()>0);
	//printf(1,"execution complete\n");
	exit();
}

void missionaryArrives(void * arg)
{
	lock_acquire(&mutex);
	printf(1,"Missionary %d has arrived!\n",getpid());
	lock_release(&mutex);
	while(1){
		lock_acquire(&mutex);
		if(numM==0)
		{
			if(numC==0 || numC==1)
			{
				sem_acquire(&boat);
				numM++;
				printf(1,"Missionary %d has gotten onto the boat!\n",getpid());
				lock_release(&mutex);
				texit();
			}
			else if(numC==2)
				lock_release(&mutex);
				tyield(); continue;
		}
		else if(numM==1)
		{
				if(numC==0)
				{
					sem_acquire(&boat);
					numM++;
					printf(1,"Missionary %d has gotten onto the boat!\n",getpid());
					lock_release(&mutex);
					texit();
				}
				else if(numC==1)
				{
					sem_acquire(&boat);
					numM++;
					printf(1,"Missionary %d has gotten onto the boat!\n",getpid());
					rowboat();
					lock_release(&mutex);
					texit();
				}
		}
		else if(numM==2 && numC==0)	//numC==0 is safety codition
		{
			sem_signal(&boat);
			numM++;
			printf(1,"Missionary %d has gotten onto the boat!\n",getpid());
			rowboat();
			lock_release(&mutex);
			texit();
		}
		else
		{
			lock_release(&mutex);
			tyield();continue;
		}
	}
}

void cannibalArrives(void * arg)
{
	lock_acquire(&mutex);
	printf(1,"Cannibal %d has arrived!\n",getpid());
	lock_release(&mutex);
	while(1)
	{
		lock_acquire(&mutex);
		if(numC==0)
		{
				if(numM==0 || numM==1)
				{
					sem_acquire(&boat);
					numC++;
					printf(1,"Cannibal %d has gotten onto the boat!\n",getpid());
					lock_release(&mutex);
					texit();
				}
				else if(numM==2)
				{
					sem_acquire(&boat);
					numC++;
					printf(1,"Cannibal %d has gotten onto the boat!\n",getpid());
					rowboat();
					lock_release(&mutex);
					texit();
				}
				else
				{
					lock_release(&mutex);
					tyield();continue;
				}
		}
		else if(numC==1)
		{
				if(numM==0)
				{
					sem_acquire(&boat);
					numC++;
					printf(1,"Cannibal %d has gotten onto the boat!\n",getpid());
					lock_release(&mutex);
					texit();
				}
				else
				{
					lock_release(&mutex);
					tyield(); continue;
				}
		}
		else if(numC==2)
		{
			if(numM==0)	//just a precaution
			{
				sem_acquire(&boat);
				numC++;
				printf(1,"Cannibal %d has gotten onto the boat!\n",getpid());
				rowboat();
				lock_release(&mutex);
				texit();
			}
			else
			{
				lock_release(&mutex);
				tyield(); continue;
			}
		}
		else{
			lock_release(&mutex);
			tyield(); continue;
		}
	}
}
void rowboat()
{
	numM=0;
	numC=0;
	printf(1,"The boat has crossed over\n~~~~~~~~\\ooo/~~~~~~~~\n~~~~~~~~~~~~~~~~~~~~~\n");
	sem_signal(&boat);
	sem_signal(&boat);
	sem_signal(&boat);
}
