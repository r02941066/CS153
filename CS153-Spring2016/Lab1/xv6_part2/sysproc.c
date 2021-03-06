#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int sys_waitpid()
{
  int pid;
  int * status;
  int options;
  argint(0,&pid);
  argptr(0,(char**) &status, sizeof(int*));
  argint(0,&options);
  return waitpid(pid, status, options);
  return 0;//never reaches here
}
int sys_change_priority()
{
  int priority;
  argint(0,&priority);
  change_priority(priority);
  return 0;
}
int
sys_exit() //ADD_USER:int ext_stat has been added to check effect
{
  int status;
  argint(0, &status);
  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
  int* status;
  argptr(0,(char **) &status, sizeof(int*));
  return wait(status);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
