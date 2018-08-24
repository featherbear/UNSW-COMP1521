// COMP1521 18s1 Week 02 Lab
// Subtract two numbers (numbers can be LARGE)

#include <stdio.h>
#include "BigNum.h"

int main(int argc, char **argv)
{
   BigNum num1;  // first input number
   BigNum num2;  // second input number
   BigNum diff;   // num1 - num2

   if (argc < 3) {
      printf("Usage: %s Num1 Num2\n", argv[0]);
      return 1;
   }

   // Initialise BigNum objects
   initBigNum(&num1, 20);
   initBigNum(&num2, 20);
   initBigNum(&diff,  20);


//   printf("argc is %d: \n", argc);
//   int i;
//   for (i=0;i<argc;i++) printf("argv[%d]: %s\n", i, argv[i]);


   // Extract values from cmd line args
   if (!scanBigNum(argv[1], &num1)) {
      printf("First number invalid\n");
      return 1;
   }

   if (!scanBigNum(argv[2], &num2)) {
      printf("Second number invalid\n");
      return 1;
   }

   // Subtract num1 - num2, store result in diff
   subtractBigNums(num1, num2, &diff);

   printf("Difference between "); showBigNum(num1);
   printf(" and "); showBigNum(num2);
   printf(" is "); showBigNum(diff);
   printf("\n");

   return 0;

}
