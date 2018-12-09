struct thread_mutex {
  uint locked;
  char *name;
  struct cpu *cpu;
  uint pcs[10];
};

uint my_xchg(uint *addr, uint newval) {
  uint result;

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}


void thread_mutex_init(struct thread_mutex *lock) {
  lock->locked = 0;
  lock->name = 0;
  lock->cpu = 0;
}


void thread_mutex_lock(struct thread_mutex *lock) {
  while(my_xchg(&lock->locked, 1))
    sleep(1);

  __sync_synchronize();
}


void thread_mutex_unlock(struct thread_mutex *lock) {
  lock->locked = 0;
}
