#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {

    int number_of_elements;
    printf("How many numbers? ");
    scanf("%d", &number_of_elements);

    int *numbers = malloc(number_of_elements);

    for (int i = 0; i < number_of_elements; i++) {
        printf("Number #%d is? ", i + 1);
        scanf("%d", &numbers[i]);
    }

    printf("Got it!\nThe numbers were: ");
    for (int i = 0; i < number_of_elements; i++) {
        printf("%d ", numbers[i]);
    }

    putchar('\n');
    return 0;
}
