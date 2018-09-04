// myls.c ... my very own "ls" implementation

#include <stdlib.h>
#include <stdio.h>
#include <bsd/string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXDIRNAME 100
#define MAXFNAME   200
#define MAXNAME    20


char *rwxmode(mode_t, char *);
char *username(uid_t, char *);
char *groupname(gid_t, char *);

char objType(mode_t st_mode);

int comp(const void* S1, const void* S2) {
  char* s1 = (char*)S1;
  char* s2 = (char*)S2;
  for (int i = 0; i < MAXFNAME; i++) {
    if (s1[i] == '\0' || s2[i] == '\0') return 0;

    if (s1[i] == s2[i]) continue;
    if (s1[i] > s2[i]) return 1;
    return -1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
   // string buffers for various names
   char dirname[MAXDIRNAME];
   char uname[MAXNAME+1]; // UNCOMMENT this line
   char gname[MAXNAME+1]; // UNCOMMENT this line
   char mode[10];

   // collect the directory name, with "." as default
   if (argc < 2)
      strlcpy(dirname, ".", MAXDIRNAME);
   else
      strlcpy(dirname, argv[1], MAXDIRNAME);

   // check that the name really is a directory
   struct stat info;
   if (stat(dirname, &info) < 0)
      { perror(argv[0]); exit(EXIT_FAILURE); }
   if ((info.st_mode & S_IFMT) != S_IFDIR)
      { fprintf(stderr, "%s: Not a directory\n",argv[0]); exit(EXIT_FAILURE); }



   char filenames[999][MAXFNAME] = {{'\0'}};
   // open the directory to start reading
   DIR *df; // UNCOMMENT this line
   df = opendir(dirname);
   if (df == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
   }

   // read directory entries
   struct dirent *entry; // UNCOMMENT this line
   struct stat file;

   int i = 0;
   while ((entry = readdir(df))) {
       if (entry->d_name[0] == '.') continue;
       strcpy(filenames[i++], entry->d_name);
   }

   qsort(filenames, sizeof(filenames)/sizeof(*filenames), sizeof(*filenames), comp);

   for (int j = 0; j < i; j++) {
       char fp[MAXFNAME];
       strcpy(fp, dirname);
       strcat(fp, "/");
       strcat(fp, filenames[j]);
       // strcat(fp, entry->d_name);

       lstat(fp, &file); // use lstat and not stat to view the actual symlink file

       char objectType = objType(file.st_mode);
       // struct stat_ file_;
       char realLocation[999] = {'\0'};
       if (objectType == 'l') realpath(filenames[j], realLocation);
       printf("%c%s  %-8.8s %-8.8s %8lld  %s%s%s\n",
               objectType,
                 rwxmode(file.st_mode, mode),
                   username(file.st_uid, uname),
                          groupname(file.st_gid, gname),
                                 (long long) file.st_size,
                                         // entry->d_name);
                                         filenames[j],
                                         (objectType == 'l') ? " -> " : "",
                                         (objectType == 'l') ? realLocation : "");
   }

   // finish up
   closedir(df);
   return EXIT_SUCCESS;
}

char objType(mode_t st_mode) {
    switch (st_mode & S_IFMT) {
        case S_IFDIR:
            return 'd';
        case S_IFREG:
            return '-';
        case S_IFLNK:
            return 'l';
        default:
            return '?';
    }
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode(mode_t mode, char *str)
{
   int i = 0;
   str[i++] = (mode & S_IRUSR) ? 'r' : '-';
   str[i++] = (mode & S_IWUSR) ? 'w' : '-';
   str[i++] = (mode & S_IXUSR) ? 'x' : '-';
   str[i++] = (mode & S_IRGRP) ? 'r' : '-';
   str[i++] = (mode & S_IWGRP) ? 'w' : '-';
   str[i++] = (mode & S_IXGRP) ? 'x' : '-';
   str[i++] = (mode & S_IROTH) ? 'r' : '-';
   str[i++] = (mode & S_IWOTH) ? 'w' : '-';
   str[i++] = (mode & S_IXOTH) ? 'x' : '-';
   str[i++] = '\0';

   return str;
}

// convert user id to user name
char *username(uid_t uid, char *name)
{
   struct passwd *uinfo = getpwuid(uid);
   if (uinfo == NULL)
      snprintf(name, MAXNAME, "%d?", (int)uid);
   else
      snprintf(name, MAXNAME, "%s", uinfo->pw_name);
   return name;
}

// convert group id to group name
char *groupname(gid_t gid, char *name)
{
   struct group *ginfo = getgrgid(gid);
   if (ginfo == NULL)
      snprintf(name, MAXNAME, "%d?", (int)gid);
   else
      snprintf(name, MAXNAME, "%s", ginfo->gr_name);
   return name;
}

