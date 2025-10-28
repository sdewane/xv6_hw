

#include "kernel/types.h"
#include "user/user.h"

int main(void) {
    int pid = fork();

    if (pid == 0) {
        sleep(5); 
        

        for(int i = 0; i < 100000; i++){} 
        
        exit(0);
    } else {

        int wait_start = uptime();
        

        while (uptime() < wait_start + 10) {} 

        printf("\n--- PS output at Tick %d ---\n", uptime());
        

        char *argv[] = { "ps", 0 };
        exec("ps", argv);
        
        wait(0);
        exit(0);
    }
}