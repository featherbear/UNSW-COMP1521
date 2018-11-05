#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char **strstr = malloc(sizeof(char*) * 10);
    

    printf("%p %p %p %p %p %p %p", strstr, strstr[0], strstr[1], strstr[2], strstr[3], strstr + 0, strstr + 1);

    return 0;
    strcpy(*(strstr+20), "String one");
  
    strcpy(*(strstr+1), "String two");
    strcpy(*(strstr+2), "String three");
    strstr[3] = NULL;
    strcpy(*(strstr+4), "String four"); 

    for (int i = 0; strstr[i]; i++) {
        printf("String inside strstr[%d] is %s\n", i, strstr[i]);
    }
}
