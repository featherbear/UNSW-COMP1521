#include <stdio.h>
#include <unistd.h>

int main() {
    char ch;
    while (read(0, &ch, 1) != 0) write(1, &ch, 1);
    // while ((ch = getchar()) != EOF) putchar(ch);
}