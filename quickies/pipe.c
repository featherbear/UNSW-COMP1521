#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
// #include <sys/types.h>

int main(void)
{
   int fd[2], pid;  char buffer[30];
   assert(pipe(fd) == 0);
   pid = fork();
   assert(pid >= 0);
   if (pid != 0) {  // parent
//      close(fd[0]); // writer; don't need fd[0]
      write(fd[1], "123456789", 10);
      write(fd[1], "987654321", 10);
   }
   else {           // child
  //    close(fd[1]); // reader; don't need fd[1]
      read(fd[0], buffer, 10);
      printf("got \"%s\"\n", buffer);

//      read(fd[0], buffer, 10);
  //    printf("got \"%s\"\n", buffer);

   }
   return 0;
}
