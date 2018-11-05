#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

int main(int argc, char **argv, char **envp) {
    char string[50];
    fgets(string, 50, stdin);

    void *stringPtr = (char*) string;
    while (*stringPtr != NULL) {
        putchar(*(stringPtr++));
    }

    return 0;
}
