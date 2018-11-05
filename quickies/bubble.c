// Bubble Sort
#include<stdio.h>

#define TRUE 1
#define FALSE 0

int main() {
    int array[] = {1, 3, 2, 5, 7, 1};

    int swapped = FALSE;
    int buffer = 0;

    int i = 1;
    while (i < 6) {

        if (array[i] < array[i - 1]) {
            buffer = array[i];
            array[i] = array[i - 1];
            array[i - 1] = buffer;
            swapped = TRUE;
            for(int j = 0; j < 6; j++) {
                printf("%d ", array[j]);
            }
            putchar('\n');
        }

        if (i == 5) {
            if (swapped == FALSE) break;
            else i = 1;
            swapped = FALSE;
        }
        i++;
    }


}