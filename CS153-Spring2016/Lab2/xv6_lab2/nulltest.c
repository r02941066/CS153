#include "types.h"
#include "user.h"

int main()
{
int* a=0;
printf(1,"Accessing uninitialized pointer %d\n", *a);
exit();
}
