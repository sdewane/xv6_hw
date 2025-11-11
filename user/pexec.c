#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define MAXARGS 16

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Usage: pexec <priority> <cmd> [args...]\n");
    exit(-1);
  }

  int prio = atoi(argv[1]);
  char *newargv[MAXARGS];
  int i;

  for (i = 2; i < argc && i - 2 < MAXARGS - 1; i++)
    newargv[i - 2] = argv[i];
  newargv[i - 2] = 0;

  int p[2];
  if (pipe(p) < 0) {
    printf("pexec: pipe failed\n");
    exit(-1);
  }

  int t_submit = uptime();

  int pid = fork();
  if (pid < 0) {
    printf("pexec: fork failed\n");
    exit(-1);
  }

  if (pid == 0) {
    setpriority(prio);


    close(p[0]);
    char b = 'X';
    write(p[1], &b, 1);
    close(p[1]);

    exec(newargv[0], newargv);
    printf("pexec: exec %s failed\n", newargv[0]);
    exit(-1);
  }


  close(p[1]); 
  char b;
  if (read(p[0], &b, 1) != 1) {
    printf("pexec: did not receive start signal\n");
    close(p[0]);
    wait(0);
    exit(-1);
  }
  close(p[0]);

  int t_first = uptime();   
  int resp = t_first - t_submit;

  wait(0);
  int t_done = uptime();
  int turn = t_done - t_submit;

  printf("PEXEC metrics: prio=%d  response=%d ticks  turnaround=%d ticks\n",
         prio, resp, turn);
  exit(0);
}