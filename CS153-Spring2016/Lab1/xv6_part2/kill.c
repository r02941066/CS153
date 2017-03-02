#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  int i;

  if(argc < 1){
    printf(2, "usage: kill pid...\n");
    exit(1); //kill syscall failed
  }
  for(i=1; i<argc; i++)
    kill(atoi(argv[i]));
  exit(0);  //success
}
