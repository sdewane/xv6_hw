#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;
  struct proc *p = myproc();

  if(argint(0, &n) < 0)
    return -1;

  addr = p->sz;
  if (n == 0)
    return addr;

  uint64 new_sz = addr + n;
  if(new_sz < p->sz){
    return (uint64)-1;
  }
  p->sz = new_sz;
  /*old eager allocatoin, we don't call growproc right away for lazy allocatoin*/
  /*if(growproc(n) < 0)
    return -1;*/
  return addr;
}

uint64
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

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_freepmem(void)
{
  uint64 pages = kfreepages_count();
  return pages * PGSIZE;
}

//hw5 task 3

uint64
sys_sem_init(void)
{
  uint64 uaddr;       // user-space address of sem_t
  int pshared;
  int value;

  if (argaddr(0, &uaddr) < 0)
    return -1;
  if (argint(1, &pshared) < 0)
    return -1;
  if (argint(2, &value) < 0)
    return -1;

  // allocate a semaphore slot in the kernel table
  int idx = semalloc();
  if (idx < 0)
    return -1;

  struct semaphore *s = &semtable.sem[idx];
  acquire(&s->lock);
  s->count = value;
  s->valid = 1;
  release(&s->lock);

  // write the index back into user memory (the sem_t)
  sem_t ksem = idx;
  if (copyout(myproc()->pagetable, uaddr, (char *)&ksem, sizeof(ksem)) < 0) {
    // if copyout fails, free the slot
    semdealloc(idx);
    return -1;
  }

  return 0;
}

uint64
sys_sem_destroy(void)
{
  uint64 uaddr;   // user-space address of sem_t
  sem_t ksem;

  if (argaddr(0, &uaddr) < 0)
    return -1;

  // read sem index from user memory
  if (copyin(myproc()->pagetable, (char *)&ksem, uaddr, sizeof(ksem)) < 0)
    return -1;

  // invalidate this semaphore in the table
  semdealloc(ksem);
  return 0;
}

uint64
sys_sem_wait(void)
{
  uint64 uaddr;
  sem_t ksem;

  if (argaddr(0, &uaddr) < 0)
    return -1;

  if (copyin(myproc()->pagetable, (char *)&ksem, uaddr, sizeof(ksem)) < 0)
    return -1;

  struct semaphore *s = &semtable.sem[ksem];

  acquire(&s->lock);
  while (s->count == 0) {
    // sleep while holding s->lock; sleep will release and reaquire it
    sleep(s, &s->lock);
  }
  s->count--;
  release(&s->lock);

  return 0;
}

uint64
sys_sem_post(void)
{
  uint64 uaddr;
  sem_t ksem;

  if (argaddr(0, &uaddr) < 0)
    return -1;

  if (copyin(myproc()->pagetable, (char *)&ksem, uaddr, sizeof(ksem)) < 0)
    return -1;

  struct semaphore *s = &semtable.sem[ksem];

  acquire(&s->lock);
  s->count++;
  // wake up any sleepers on this semaphore
  wakeup(s);
  release(&s->lock);

  return 0;
}