struct thread_spinlock {
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


void thread_spin_init(struct thread_spinlock *lock) {
  lock->locked = 0;
  lock->name = 0;
  lock->cpu = 0;
}


void thread_spin_lock(struct thread_spinlock *lock) {
  while(my_xchg(&lock->locked, 1));

  __sync_synchronize();
}


void thread_spin_unlock(struct thread_spinlock *lock) {
  lock->locked = 0;
}
