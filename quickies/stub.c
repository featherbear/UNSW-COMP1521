#include <stdio.h>

int main() {
  int x = 1;
  int* y = &x;

  printf("%p\n", &y);
  return 0;
}
