#define DEBUG 0
// BigNum.h ... LARGE positive integer values

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "BigNum.h"

void zeroFill(BigNum *n) {
    int i;
    for (i = 0; i < n->nbytes; i++) n->bytes[i] = 0;
}

int realLength(BigNum n) {
    int i, j = 0;
    int length = 0;
    for (i = 0; i < n.nbytes; i++) if ((!j && n.bytes[i] && j++) || (j && length++));
    return length;
}

int isBigger(BigNum n, BigNum m) {
    int rl_n = realLength(n);
    int rl_m = realLength(m);

    if (rl_n > rl_m) return 1;

    if (rl_n == rl_m) {
       int i; // n.nbytes - rl_n;
       for (i = 0; i < rl_n; i++) {
           if (n.bytes[n.nbytes-rl_n+i] == m.bytes[m.nbytes-rl_n+i]) continue;
           return n.bytes[n.nbytes-rl_n+i] > m.bytes[m.nbytes-rl_n+i];
       }
    }

    return 0;
}

int isEqual(BigNum n, BigNum m) {
    int rl_n = realLength(n);
    int rl_m = realLength(m);

    if (rl_n != rl_m) return 0;

    int i; // = n.nbytes - rl_n;
    for (i = 0; i < rl_n; i++) {
        if (n.bytes[n.nbytes - rl_n + i] == m.bytes[m.nbytes - rl_n + i]) continue;
        return 0;
    }
    return 1;
}

void takeTen(Byte *a, int index) {
    if (a[index] == 0) {
        takeTen(a, index - 1);
        a[index] = 9;
    } else {
        a[index]--;
    }
}

void spillCheck(Byte *a, int index) {
    if (DEBUG) printf("Spill check on %d\n", a[index]);
    if (a[index] / 10) {
        if (DEBUG) printf("  Spill found\n");
        a[index - 1] += a[index] / 10;
        a[index] %= 10;
        spillCheck(a, index - 1);
    }
}

// Initialise a BigNum to N bytes, all zero
void initBigNum(BigNum *n, int Nbytes) {
    n->nbytes = Nbytes;

    Byte *ptr = malloc(Nbytes * sizeof(Byte));
    assert(ptr != NULL);
    n->bytes = ptr;
    zeroFill(n);
}

// Add two BigNums and store result in a third BigNum
void addBigNums(BigNum n, BigNum m, BigNum *res) {
    int rl_n = realLength(n);
    int rl_m = realLength(m);

    // Length of the result will be at max the longest + 1
    int size = (rl_n > rl_m ? rl_n : rl_m) + 1;
    initBigNum(res, size);

    // Copy first big number to the result
    int i;
    for (i = 1; i <= rl_n; i++) {
        res->bytes[res->nbytes - i] = n.bytes[n.nbytes - i];
    }

    // Add second big number to the result, and check for spills
    for (i = 1; i <= rl_m; i++) {
        res->bytes[res->nbytes - i] += m.bytes[m.nbytes - i];
        spillCheck(res->bytes, res->nbytes - i);
    }

    /*
        int size = (n.nbytes > m.nbytes ? n.nbytes : m.nbytes) + 1;
        // Length of n, but index 0...n-1
        size--;
        int i = n.nbytes - 1, j = m.nbytes - 1;

        int carry = 0; // Number spillage

         // Add the columns, starting from the last index
         for (; size >= 0; i--, j--) {
            int val = (i >= 0 ? n.bytes[i] : 0) + (j >= 0 ? m.bytes[j] : 0) + (carry ? !(carry = 0) : 0);
            if (val / 10) {
                carry = 1;
                val = val % 10;
            }
            res->bytes[size--] = val;
        }

        if (carry) {
            res->bytes[size]++;
        }
    */
}

// Subtract two BigNums and store result in a third BigNum
// What do we do with negatives?
void subtractBigNums(BigNum n, BigNum m, BigNum *res) {
    int rl_n = realLength(n);
    int rl_m = realLength(m);

    // Length of the result will be at max the longest
    int size = (rl_n > rl_m ? rl_n : rl_m);
    initBigNum(res, size);

    int f = isBigger(n, m);
    BigNum bigger = f ? n : m;
    BigNum lesser = f ? m : n;

    // Copy bigger number to the result
    int i;
    for (i = 1; i <= (f ? rl_n : rl_m); i++) {
        res->bytes[res->nbytes - i] = bigger.bytes[bigger.nbytes - i];
    }

    for (i = 1; i <= (f ? rl_m : rl_n); i++) {
        if (res->bytes[res->nbytes - i] < lesser.bytes[lesser.nbytes - i]) {
            takeTen(res->bytes, res->nbytes - i - 1);
            res->bytes[res->nbytes - i] += 10;
        }
        res->bytes[res->nbytes - i] -= lesser.bytes[lesser.nbytes - i];
    }

    // if (rl_m > rl_n || (rl_m == rl_n && ) )
    // Subtract second big number to the result, and check for spills
//    for (i = 1; i <= rl_m; i++) {
//        if (m.bytes[m.nbytes - i] > res->bytes[res->nbytes - i]) {
//            if(!m.bytes[m.nbytes - i - 1])
//            m.bytes[m.nbytes - i - 1] -= 1;
//            m.bytes[m.nbytes - i] += 10;
//        }
//        res->bytes[res->nbytes - i] -= m.bytes[m.nbytes - i];
//        spillCheck(res->bytes, res->nbytes - i);
//    }
}


void multiplyBigNums(BigNum n, BigNum m, BigNum *res) {
    int rl_n = realLength(n);
    int rl_m = realLength(m);

    if (DEBUG) printf("Real length of n is %d\n", rl_n);
    if (DEBUG) printf("Real length of m is %d\n", rl_m);

    int size = rl_m + rl_n;
    if (DEBUG) printf("Max length of a %d x %d product is %d\n", rl_n, rl_m, size);

    initBigNum(res, size);

    int i, j, c1, c2;

    for (i = 0; i < rl_n; i++) {
        int index = res->nbytes;
        c1 = n.bytes[n.nbytes - 1 - i];
        if (c1 == 0) continue;
        for (j = 1; j <= rl_m; j++) {
            c2 = m.bytes[m.nbytes - j];
            if (c2 == 0) continue;
            res->bytes[index - i - j] += c1 * c2;
            spillCheck(res->bytes, index - i - j);
        }
    }
}

/* ABORT ABORT ABORT ABORT ABORT ABORT */
BigNum divideBigNums(BigNum n, BigNum m, BigNum *res) {
    // dividend = n
    // divisor = m
    // quotient = res
    // BigNum *remainder;

    int rl_n = realLength(n);
    int rl_m = realLength(m);

    // Length of the result will be at max the longest
    int size = (rl_n > rl_m ? rl_n : rl_m);
    initBigNum(res, size);
    // initBigNum(remainder, size);

    BigNum *one = malloc(sizeof(BigNum));
    one->bytes = malloc(sizeof(Byte));
    one->nbytes = one->bytes[0] = 1;

    while (isBigger(n, m) || isEqual(n, m)) {
  	subtractBigNums(n, m, &n);
	addBigNums(*res, *one, res);
    }

    return n;
}

// Set the value of a BigNum from a string of digits
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum(char *s, BigNum *n) {

    int l = 0;
    while (*s == '\0' || *s == ' ') s++;
    while (s[l] != '\0' && s[l] != ' ') l++;

    if (DEBUG) printf("Length of \"%s\" is %d long\n", s, l);

    // If the string is longer than the length of BigNum, expand
    if (n->nbytes < l) {
        if (DEBUG) printf("    nbytes too little... expanding\n");
        n->bytes = realloc(n->bytes, l * sizeof(Byte));
        n->nbytes = l;
    }

    // Populate the Byte array with 0
    if (DEBUG) printf("Filling zeroes...\n");
    zeroFill(n);

    // Convert the digit string into digit numbers
    int j = n->nbytes;
    for (l--; l >= 0; l--) {
        if (s[l] < '0' || s[l] > '9') return 0; // Only allow numbers
        n->bytes[j - 1] = s[l] - '0';
        j--;
    }

    return 1;
}

// Display a BigNum in decimal format
void showBigNum(BigNum n) {
    int i;
    int j = realLength(n);

    if (!j && putchar('0'));
    for (i = n.nbytes - j; i < n.nbytes; i++) putchar('0' + n.bytes[i]);

    /*
       // Check for leading zeros
       if (j == 0 && n.bytes[i] != 0) {
          j = 1;
       } else {
          printf("%d", n.bytes[i]);
       }
       if (j == 0) {
          putchar('0');
       }
    */
}

