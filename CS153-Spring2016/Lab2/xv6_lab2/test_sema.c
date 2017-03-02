#include "types.h"
#include "user.h"

int i=0;
void doone(void*);

int main(){
    
void * arg = thread_create(doone,(void*)&i);
if(arg==0)
	printf(1,"thread create failed\n");
while(wait()>0);
printf(1,"execution complete\n");
exit();
}

void doone(void arg*)
{
	while(i<10)
	{
	printf(1,"thred yielding\n ");
	i++;
	}
	texit();
}
