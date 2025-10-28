// user/task1.c

#include "kernel/types.h"
#include "user/user.h"

#define NEW_PRIO 17

int main(void) {
    //int pid = getpid();
    int initial_prio = getpriority();
    printf("parent initial priority: %d\n", initial_prio);

    // 1. Set new priority for parent
    if (setpriority(NEW_PRIO) < 0) {
        fprintf(2, "task1: setpriority failed\n");
        exit(1);
    }
    printf("parent after set: %d\n", getpriority());

    // 2. Fork to test inheritance
    int child_pid = fork();

    if (child_pid < 0) {
        fprintf(2, "task1: fork failed\n");
        exit(1);
    } else if (child_pid == 0) { // Child process
        printf("child inherited priority: %d\n", getpriority());
        
        // 3. Exec ps to show all process info
        printf("Exec ps to print all process info\n");
        char *argv[] = { "ps", 0 };
        exec("ps", argv);
        
        // Should not reach here if exec succeeded
        fprintf(2, "task1: exec failed\n");
        exit(1);
    } else { // Parent process
        // Parent waits for the child to finish
        wait(0); 
        exit(0);
    }
}