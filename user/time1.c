#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"  

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: time1 command [args...]\n");
    exit(1);
  }

  struct rusage r;
  int start = uptime();

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    exec(argv[1], &argv[1]);
    fprintf(2, "exec %s failed\n", argv[1]);
    exit(1);
  }

  // we don't need child's exit status → pass 0
  wait2(0, &r);

  int end = uptime();
  int elapsed = end - start;
  int cpu = r.cputime;
  int pct = (elapsed > 0) ? (cpu * 100) / elapsed : 0;

  printf("elapsed time: %d ticks , cpu time: %d ticks , %d%% CPU\n",
         elapsed, cpu, pct);
  exit(0);
}