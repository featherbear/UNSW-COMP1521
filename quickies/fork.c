#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    wait(NULL);

    pid = fork();
    if (pid != 0) {
        wait(NULL);
        printf("I am the parent.\n");

    }
    else {
        printf("I am the child.\n");
        exit(0);
    }
    return 0;
}