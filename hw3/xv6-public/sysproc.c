#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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


int sys_backtrace(void) {
  struct proc *proc = myproc();

  cprintf("eax: %p\n", proc->tf->eax);
  cprintf("ebx: %p\n", proc->tf->ebx);
  cprintf("ecx: %p\n", proc->tf->ecx);
  cprintf("edx: %p\n", proc->tf->edx);
    
  cprintf("edi: %p\n", proc->tf->edi);
  cprintf("esi: %p\n", proc->tf->esi);
  cprintf("ebp: %p\n", proc->tf->ebp);
  cprintf("esp: %p\n", proc->tf->esp);
  cprintf("eip: %p\n", proc->tf->eip);


  uint *pp = (uint *)proc->tf->ebp;
  register int ebp asm("ebp");
  pp = (uint *)ebp;
  int j = 0;

  for(;*(pp + 1) != 0xFFFFFFFF;
      pp = *(uint **)pp, ++j) {
    cprintf("%d: 0x%p\n", j, *(pp + 1));
  }
  cprintf("%d: 0x%p\n", j, *(pp + 1));

  return 0;
}


int sys_getprocinfo(void) {
  int proc_num;
  struct uproc *uproc = 0;

  if(argint(0, &proc_num) < 0)
    return -1;

  if(argint(1, (int *) &uproc) < 0)
    return -1;


  volatile struct proc proc = get_proc()[proc_num];
  uproc->sz = proc.sz;
  uproc->state = proc.state;
  uproc->pid = proc.pid;
  uproc->parent_pid = proc.parent->pid;
  uproc->killed = proc.killed;

  for(int i = 0; i < 16; ++i)
    uproc->name[i] = proc.name[i];

  return 0;
}
