#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void spin(const char *tag, int loops){
  for (int i = 0; i < loops; i++) {
    if ((i % 1000000) == 0) printf("%s %d\n", tag, i);
  }
}

int main(void){
  printf("parent priority (before): %d\n", getpriority());
  setpriority(5);
  printf("parent priority (after): %d\n", getpriority());

  int pid = fork();
  if (pid < 0) { printf("fork failed\n"); exit(1); }

  if (pid == 0) {
    setpriority(0);
    printf("child priority: %d\n", getpriority());
    spin("low", 20000000);
    exit(0);
  }

  setpriority(30);
  printf("parent boosted priority: %d\n", getpriority());
  spin("high", 20000000);
  wait(0);
  exit(0);
}
