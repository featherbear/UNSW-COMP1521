#include <glob.h>
#include <stdio.h>

int main() {

    glob_t gb;
    glob("./program *.s *c", GLOB_NOCHECK, NULL, &gb);
    printf("Matched: %d\n", gb.gl_pathc);
    for (char ** a = gb.gl_pathv; *a; a++) {
        printf("%s\n", *a);
    }

}
