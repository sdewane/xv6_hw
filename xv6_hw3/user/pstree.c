#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

static const char *state2str(int s) {
  switch (s) {
    case SLEEPING_C: return "sleeping";
    case RUNNABLE_C: return "runnable";
    case RUNNING_C:  return "running";
    case ZOMBIE_C:   return "zombie";
    default:         return "unknown";
  }
}

static struct pstat procs[NPROC];
static int nprocs;

static int find_idx_by_pid(int pid) {
  for (int i = 0; i < nprocs; i++)
    if (procs[i].pid == pid) return i;
  return -1;
}

static void mktree(int indent, int pid) {
  int idx = find_idx_by_pid(pid);
  if (idx < 0) {
    printf("pid %d not found\n", pid);
    return;
  }

  for (int j = 0; j < indent; j++) printf("  ");
  printf("%s(%d) state=%s prio=%d\n",
         procs[idx].name, procs[idx].pid,
         state2str(procs[idx].state), procs[idx].priority);

  // collect childern
  int kids[NPROC], kc = 0;
  for (int i = 0; i < nprocs; i++)
    if (procs[i].ppid == pid) kids[kc++] = procs[i].pid;

  // sort kids pid
  for (int i = 1; i < kc; i++) {
    int key = kids[i], j = i - 1;
    while (j >= 0 && kids[j] > key) { kids[j+1] = kids[j]; j--; }
    kids[j+1] = key;
  }

  for (int i = 0; i < kc; i++) mktree(indent + 1, kids[i]);
}

int main(int argc, char **argv) {
  int root = 1;
  if (argc == 2) root = atoi(argv[1]);

  nprocs = getprocs(procs);      
  if (nprocs < 0) {
    printf("pstree: getprocs failed\n");
    exit(1);
  }

  mktree(0, root);
  exit(0);
}
