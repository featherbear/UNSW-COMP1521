#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
   FILE *fp, *pp; char line[100];
   fp = fopen("file1","r");
   assert(fp != NULL);
   pp = popen("wc -l","w");
   assert(pp != NULL);
   while (fgets(line,100,fp) != NULL)
      fputs(line,pp);
   fclose(fp); pclose(pp);
}
