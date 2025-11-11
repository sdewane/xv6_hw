#define NPROC        64
#define NCPU          8
#define NOFILE       16
#define NFILE       100
#define NINODE       50
#define NDEV         10
#define ROOTDEV       1
#define MAXARG       32
#define MAXOPBLOCKS  10
#define LOGSIZE      (MAXOPBLOCKS*3)
#define NBUF         (MAXOPBLOCKS*3)
#define FSSIZE       1000
#define MAXPATH      128

// Scheduling policy selectors
#define DEFAULT_SCHED   0
#define PRIORITY_SCHED  1

#ifndef SCHED_ALGO
#define SCHED_ALGO DEFAULT_SCHED
#endif

// Aging / priority tunables defined
#ifndef MAXPRIO
#define MAXPRIO 49
#endif

#ifndef AGING_TICKS
#define AGING_TICKS 50
#endif



//enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };