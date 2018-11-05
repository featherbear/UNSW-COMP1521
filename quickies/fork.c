#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;  int stat;

    if ((pid = fork()) != 0) {
        printf("+ I am the parent with PID %d.\n", getpid());
        printf("+ The value of `pid` is %d.\n", pid);
        printf("+ I created a child with PID %d.\n", pid);
        int somePID = wait(&stat);
        printf("+ A process with PID %d exited with code: %d\n", somePID, stat);
        return 1;
    }
    else {
        printf("- I am a child process with PID %d.\n", getpid());
        printf("- The value of `pid` is %d.\n", pid);
        printf("- My parent process PID is: %d\n", getppid());
        return 0;
    }
    return 0;
}
