enum procstate_copy { UNUSED_C, USED_C, SLEEPING_C, RUNNABLE_C, RUNNING_C, ZOMBIE_C };

struct pstat {
  int pid;     // Process ID
  enum procstate_copy state;  // Process state
  uint64 size;     // Size of process memory (bytes)
  int ppid;        // Parent process ID
  char name[16];   // Parent command name
  int priority;    // hw3
};