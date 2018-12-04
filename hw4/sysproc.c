#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

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

  cprintf("Registers\n");
  cprintf("eax: 0x%p\n", proc->tf->eax);
  cprintf("ebx: 0x%p\n", proc->tf->ebx);
  cprintf("ecx: 0x%p\n", proc->tf->ecx);
  cprintf("edx: 0x%p\n", proc->tf->edx);
    
  cprintf("edi: 0x%p\n", proc->tf->edi);
  cprintf("esi: 0x%p\n", proc->tf->esi);
  cprintf("ebp: 0x%p\n", proc->tf->ebp);
  cprintf("esp: 0x%p\n", proc->tf->esp);
  cprintf("eip: 0x%p\n\n", proc->tf->eip);

  cprintf("Return Addresses\n");
  //top of user stack
  //uint *pp = (uint *)proc->tf->ebp;
  uint *pp = 0;

  //top of kernel stack
  register uint ebp asm("ebp");

  //address where stack starts
  uint lower = (ebp / 4096) * (ebp + 1);

  pp = (uint *)ebp;
  int j = 0;

  cprintf("Kernel Stack\n");

  for(;pp > (uint *)lower;
      pp = *(uint **)pp, ++j) {
    cprintf("%d: 0x%p\n", j, *(pp + 1));
  }

  cprintf("User Stack\n");

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

  int i = 0;
  for(; i < 16; ++i)
    uproc->name[i] = proc.name[i];

  return 0;
}


int sys_thread_create(void) {
  void (*fcn)(void *) = 0;
  void *arg = 0;
  void *stack = 0;

  if(argint(0, (int *) &fcn) < 0)
    return -1;

  if(argint(1, (int *) &arg) < 0)
    return -1;

  if(argint(2, (int *) &stack) < 0)
    return -1;

  return thread_create(fcn, arg, stack);
}

int sys_thread_join(void) {
  return thread_join();
}

int sys_thread_exit(void) {
  return thread_exit();
}
