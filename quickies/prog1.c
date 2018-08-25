#define LIFE 42
#define VAL random() % 20
#define sq(x) (x*x)
#define woof(y) (LIFE+y)

#include <stdio.h>

int main() {
  char s[LIFE];
  int i = woof(5);
  i = VAL;
  int x = (sq(i) > LIFE) ? 1 : 0;
  printf("%d", x);
  return x;
}
