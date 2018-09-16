#include <stdio.h>
int main() {
    while (read(0, &ch, 1) != 0)
    write(1, &ch, 1);
}