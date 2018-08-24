// Copy input to output
// COMP1521 18s1

#include <stdlib.h>
#include <stdio.h>

void copy(FILE *, FILE *);

int main(int argc, char *argv[])
{
    if (argc != 1) {
        for(int i = 1; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr, "%s %s %s", "Can't read", argv[i], "\n");
                // Can use perror here - but concat?
            } else {
                copy(file, stdout);
                fclose(file);
            }
        }
    } else {
    	copy(stdin,stdout);
    }
	return EXIT_SUCCESS;
}

// Copy contents of input to output, char-by-char
// Assumes both files open in appropriate mode

void copy(FILE *input, FILE *output)
{
    char line[BUFSIZ] = {'\0'};
    while (fgets(line, BUFSIZ, input) != NULL) {
        fputs(line, output);
    }
}
