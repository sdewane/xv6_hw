#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

static const char *state2str(int s) {
  switch (s) {
    case SLEEPING_C: return "sleeping";
    case RUNNABLE_C: return "runnable";
    case RUNNING_C:  return "running ";
    case ZOMBIE_C:   return "zombie  ";
    default:         return "unknown ";
  }
}

int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
  int nprocs;

  // getprocs expects a user pointer (uint64), so cast the array address
  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  printf("pid\tstate\t\tsize\tppid\tpriority\tname\n");
  for (int i = 0; i < nprocs; i++) {
    printf("%d\t%s\t%lu\t%d\t%d\t\t%s\n",
           uproc[i].pid,
           state2str(uproc[i].state),
           (unsigned long)uproc[i].size,   // size is uint64 -> use %lu
           uproc[i].ppid,
           uproc[i].priority,
           uproc[i].name);
  }

  exit(0);
}
