#include "types.h"
#include "user.h"

int i=0;
void doone(void* arg );

int main(){

void * arg = thread_create(doone,(void*)&i);
if(arg==0)
	printf(1,"thread create failed\n");
void * arg1 = thread_create(doone,(void*)&i);
if(arg1==0)
	printf(1,"thread create failed\n");
void * arg2 = thread_create(doone,(void*)&i);
if(arg2==0)
	printf(1,"thread create failed\n");
//printf(1,"execution complete\n");
while(wait()>0);
exit();
}

void doone(void * arg)
{

	printf(1,"%d thread yielding\n", getpid());
	tyield();
  printf(1,"%d thread exited\n", getpid());
	texit();
}
