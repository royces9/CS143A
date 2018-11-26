#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

void print_proc(struct uproc *uproc) {
  printf(1, "%d ", uproc->pid);
  printf(1, "%s\n", uproc->name);
/*
  printf(1, "%d ", uproc->state);
  printf(1, "%d ", uproc->parent_pid);
  printf(1, "%d ", uproc->killed);
  printf(1, "%d ", uproc->sz);
*/
}

int main(int argc, char **argv) {
  struct uproc uproc;
  printf(1, "PID NAME\n");
int i = 0;  
for(; i < NPROC; ++i) {
    if(getprocinfo(i, &uproc) < 0)
      exit();

    if(uproc.sz)
      print_proc(&uproc);
  }

  exit();
}
