// where_are_the_bits.c ... determine bit-field order
// COMP1521 Lab 03 Exercise
// Written by ...

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
   unsigned int a : 4,
                b : 8,
                c : 20;
};


union combined {
    int ival;
    struct _bit_fields bits;
};


int main(void) {
    union combined un;
    un.bits.a = un.bits.b = 0;
    un.bits.c = ~0; // 0b11111111111111111111;

    printf("ival = %u\n", un.ival);

    printf("Bit read out: ");
    int i;
    for (i = sizeof(int) * 8 - 1; i >= 0; i--) {
        putchar('0' + ((un.ival >> i) & 0b1));
    };
    putchar('\n');

/*
    struct _bit_fields x;

    int index;
    unsigned char* ptr = (unsigned char*)&x;
    printf("Address of x: %p\n", &x);
    printf("Address stored in cheat: %p\n", cheat);
    printf("Size of struct: %d\n", sizeof(struct _bit_fields));

    for (index = 0; index < (sizeof(struct _bit_fields)*8); index++) {
        printf("%u ", *(ptr-index));
    }
    putchar('\n');
*/

/*
int readMemory(int i);
   int size = sizeof(x) * 8;
   for (i = 0; i < size; i++) {
      printf("%d", ((x.c >> i) & 0b1)    ? 1 : 0);
   }

   return 0;
}
*/


   return 0;
}
