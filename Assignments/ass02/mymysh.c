// mysh.c ... a small shell
// Started by John Shepherd, September 2018

// Completed by Andrew Wong (z5206677), September/October 2018
// https://github.com/featherbear/UNSW-COMP1521/tree/master/Assignments/ass02

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <glob.h>
#include <assert.h>
#include <fcntl.h>
#include "history.h"

// This is defined in string.h
// BUT ONLY if you use -std=gnu99
//extern char *strdup(char *);

// Function forward references

void trim(char *);

int strContains(char *, char *);

char **tokenise(char *, char *);

char **fileNameExpand(char **);

void freeTokens(char **);

char *findExecutable(char *, char **);

int isExecutable(char *);

void prompt(void);


// Global Constants

#define MAXLINE 200

// Global Data

/* none ... unless you want some */

void printPWD() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
}

// Main program
// Set up enviroment and then run main loop
// - read command, execute command, repeat

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;   // pid of child process
    int stat;    // return status of child
    char **path; // array of directory names
    int i;       // generic index

    // set up command PATH from environment variable
    for (i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], "PATH=", 5) == 0) break;
    }
    if (envp[i] == NULL)
        path = tokenise("/bin:/usr/bin", ":");
    else
        // &envp[i][5] skips over "PATH=" prefix
        path = tokenise(&envp[i][5], ":");
#ifdef DBUG
    for (i = 0; path[i] != NULL;i++)
       printf("path[%d] = %s\n",i,path[i]);
#endif

    // initialise command history
    // - use content of ~/.mymysh_history file if it exists
    initCommandHistory();

    // main loop: print prompt, read line, execute command
    char line[MAXLINE];
    int tokenLength = 0;
    char **tokens;
    int tokenised = 0;

    for (; prompt(), fgets(line, MAXLINE, stdin) != NULL;
           tokenLength = 0, (tokenised && (freeTokens(tokens), 1)), tokenised = 0) {
        // For each loop, print out the prompt and request input
        // At the end of each loop, set `tokenLength` and `tokenised` to 0.
        //   Additionally, call `freeTokens(tokens)` if `tokenised` was previously 1.
        //   (tokenised && (..., 1)) is a hack to use the void function `freeTokens` in an expression

        // remove leading/trailing space
        trim(line);

        // An empty line was entered, ignore
        if (*line == '\0') continue;

        // Recall command assumed
        if (line[0] == '!') {
            if (line[1] == '\0') {
                // Show the command history if the input is just `!`
                showCommandHistory();
                continue;
            }
            char *recallPtr;
            int cmdNo = 1;

            // If `!!` -> getLastCommandFromHistory
            // Else `!n` -> getCommandFromHistory(n-1)
            recallPtr = (line[1] == '!' && line[2] == '\0') ? getLastCommandFromHistory()
                                                            : getCommandFromHistory((cmdNo = atoi(line + 1)) - 1);

            // Check if the command was found
            if (recallPtr) {
                // Command found, so copy into `line`.
                strcpy(line, recallPtr);
                printf("%s\n", line);
            } else {
                printf("No command #%d\n", cmdNo);
                continue;
            }
        }

        /* TOKENISER */
        tokens = tokenise(line, " "); // Tokenise input line
        if (strContains(line, "*?[~")) tokens = fileNameExpand(tokens); // Resolve wildcards
        tokenised = 1; // Flag for freeTokens()
        while (tokens[++tokenLength] != NULL); // Count the number of tokens

        // Validate redirect tokens
        if (strContains(tokens[tokenLength - 1], "<>") || strContains(tokens[0], "<>")) {
            printf("Invalid redirection; need a file\n");
            continue;
        }

        /* Handle built-in commands */
        if (strcmp(tokens[0], "exit") == 0) {
            freeTokens(tokens);
            break;

        } else if (strcmp(tokens[0], "h") == 0 || strcmp(tokens[0], "history") == 0) {
            // Adding command to history before it is executed to show the correct recall order
            addToCommandHistory(line);
            showCommandHistory();

        } else if (strcmp(tokens[0], "pwd") == 0) {
            printPWD();
            addToCommandHistory(line);

        } else if (strcmp(tokens[0], "cd") == 0) {
            // Change directory into arg1, else $HOME
            if (chdir(tokens[1] ? tokens[1] : getenv("HOME")) == 0) {
                addToCommandHistory(line);
                printPWD();
            } else {
                perror(tokens[1]);
            }

        } else {
            /* Handle programs */
            char *exec;
            if ((exec = findExecutable(tokens[0], path))) {
                int stdin = STDIN_FILENO;
                int stdout = STDOUT_FILENO;

                /* Check for redirection */
                if (tokenLength > 2 && strContains(tokens[tokenLength - 2], "<>")) {
                    // stdin redirection
                    if (strcmp(tokens[tokenLength - 2], "<") == 0 &&
                        (stdin = open(tokens[tokenLength - 1], O_RDONLY)) == -1) {
                        perror("Input redirection");
                        continue;
                    };

                    // stdout redirection
                    if (strcmp(tokens[tokenLength - 2], ">") == 0 &&
                        (stdout = open(tokens[tokenLength - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1) {
                        perror("Output redirection");
                        continue;
                    }

                    // stdout append redirection
                    if (strcmp(tokens[tokenLength - 2], ">>") == 0 &&
                        (stdout = open(tokens[tokenLength - 1], O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1) {
                        perror("Output redirection");
                        continue;
                    }

                    // Remove the redirect and path tokens as we don't want them to be passed to the program
                    free(tokens[tokenLength - 1]);
                    free(tokens[tokenLength - 2]);
                    tokens[tokenLength - 2] = NULL;
                }

                printf("Running %s ...\n", exec);
                printf("--------------------\n");
                if ((pid = fork() != 0)) {
                    /* Parent Process */
                    // Wait for child to exit, then print return code and add the command to the command history
                    wait(&stat);
                    printf("--------------------\n");
                    printf("Returns %d\n", stat);
                    addToCommandHistory(line);
                } else {
                    /* Child Process */
                    // Redirect stdin and stdout if needed and transform process
                    if (stdin != STDIN_FILENO) dup2(stdin, STDIN_FILENO);
                    if (stdout != STDOUT_FILENO) dup2(stdout, STDOUT_FILENO);
                    execve(exec, tokens, envp);
                    return -1;
                }
            } else {
                printf("%s: Command not found\n", tokens[0]);
            }
        }
    }

    // No more commands, save, clean up and exit
    saveCommandHistory();
    cleanCommandHistory();

    freeTokens(path);

    printf("\n");
    return (EXIT_SUCCESS);
}

// fileNameExpand: expand any wildcards in command-line args
// - returns a possibly larger set of tokens
char **fileNameExpand(char **tokens) {
    // Count number of tokens
    int tokenLength = 0;
    while (tokens[++tokenLength] != NULL);

    // Initialise an array of `glob_t` structures
    glob_t *globbuf = malloc(tokenLength * sizeof(glob_t));

    // Resolve the wildcard expansions, and count the total number of tokens
    int newTokenLength = 0;
    for (int i = 0; i < tokenLength; i++) {
        glob(tokens[i], GLOB_NOCHECK | GLOB_TILDE, NULL, &globbuf[i]);
        newTokenLength += globbuf[i].gl_pathc;
    }

    // Initialise a new array of tokens
    char **newTokens = malloc((newTokenLength + 1) * sizeof(char *));

    // Populate the new token array
    int tokenCounter = 0;
    for (int i = 0; i < tokenLength; i++) {
        for (int j = 0; j < globbuf[i].gl_pathc; j++) {
            newTokens[tokenCounter++] = strdup(globbuf[i].gl_pathv[j]);
        }
    }

    // NULL terminate the token array
    newTokens[tokenCounter] = NULL;

    // Clean up
    for (int i = 0; i < tokenLength; i++) globfree(&globbuf[i]);
    free(globbuf);
    freeTokens(tokens);

    return newTokens;
}

// findExecutable: look for executable in PATH
char *findExecutable(char *cmd, char **path) {
    char executable[MAXLINE];
    executable[0] = '\0';
    if (cmd[0] == '/' || cmd[0] == '.') {
        strcpy(executable, cmd);
        if (!isExecutable(executable))
            executable[0] = '\0';
    } else {
        int i;
        for (i = 0; path[i] != NULL; i++) {
            sprintf(executable, "%s/%s", path[i], cmd);
            if (isExecutable(executable)) break;
        }
        if (path[i] == NULL) executable[0] = '\0';
    }
    if (executable[0] == '\0')
        return NULL;
    else
        return strdup(executable);
}

// isExecutable: check whether this process can execute a file
int isExecutable(char *cmd) {
    struct stat s;
    // must be accessible
    if (stat(cmd, &s) < 0) return 0;
    // must be a regular file
    //if (!(s.st_mode & S_IFREG))
    if (!S_ISREG(s.st_mode)) return 0;
    // if it's owner executable by us, ok
    if (s.st_uid == getuid() && s.st_mode & S_IXUSR) return 1;
    // if it's group executable by us, ok
    if (s.st_gid == getgid() && s.st_mode & S_IXGRP) return 1;
    // if it's other executable by us, ok
    if (s.st_mode & S_IXOTH) return 1;
    return 0;
}

// tokenise: split a string around a set of separators
// create an array of separate strings
// final array element contains NULL
char **tokenise(char *str, char *sep) {
    // temp copy of string, because strtok() mangles it
    char *tmp;
    // count tokens
    tmp = strdup(str);
    int n = 0;
    strtok(tmp, sep);
    n++;
    while (strtok(NULL, sep) != NULL) n++;
    free(tmp);
    // allocate array for argv strings
    char **strings = malloc((n + 1) * sizeof(char *));
    assert(strings != NULL);
    // now tokenise and fill array
    tmp = strdup(str);
    char *next;
    int i = 0;
    next = strtok(tmp, sep);
    strings[i++] = strdup(next);
    while ((next = strtok(NULL, sep)) != NULL)
        strings[i++] = strdup(next);
    strings[i] = NULL;
    free(tmp);
    return strings;
}

// freeTokens: free memory associated with array of tokens
void freeTokens(char **toks) {
    for (int i = 0; toks[i] != NULL; i++)
        free(toks[i]);
    free(toks);
}

// trim: remove leading/trailing spaces from a string
void trim(char *str) {
    int first, last;
    first = 0;
    while (isspace(str[first])) first++;
    last = strlen(str) - 1;
    while (isspace(str[last])) last--;
    int i, j = 0;
    for (i = first; i <= last; i++) str[j++] = str[i];
    str[j] = '\0';
}

// strContains: does the first string contain any char from 2nd string?
int strContains(char *str, char *chars) {
    for (char *s = str; *s != '\0'; s++) {
        for (char *c = chars; *c != '\0'; c++) {
            if (*s == *c) return 1;
        }
    }
    return 0;
}

// prompt: print a shell prompt
// done as a function to allow switching to $PS1
void prompt(void) {
    printf("mymysh$ ");
}
