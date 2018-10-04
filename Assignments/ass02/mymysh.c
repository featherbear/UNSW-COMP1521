#define _GNU_SOURCE

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
// Set up environment and then run main loop
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
            if (line[1] != '!') {
                if (line[1] == '\0' || line[1] < '0' || line[1] > '9') {
                    // Show error if the given string is not valid
                    printf("Invalid history substitution\n");
                    continue;
                }
            }
            char *recallPtr;
            int cmdNo = 1;

            // If `!!` -> getLastCommandFromHistory
            // Else `!n` -> getCommandFromHistory(n-1)
            recallPtr = (line[1] == '!' && line[2] == '\0') ? getLastCommandFromHistory()
                                                            : getCommandFromHistory((cmdNo = atoi(line + 1)));

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

        // Validate redirect and pipe tokens
        // ">" or "<" (mutually exclusive) can only exist as the second last token
        // "|" token cannot be be adjacent to another pipe token
        // "|" token cannot exist as the first or last token

        int validRedirect = 1;
        for (int i = 0; i < tokenLength; i++) {

            // Pipe checking
            if ((i == 0 && strcmp(tokens[0], "|") == 0)
                || (i == tokenLength - 1 && strcmp(tokens[tokenLength - 1], "|") == 0)
                || (i > 0 && strcmp(tokens[i], "|") == 0 && strcmp(tokens[i - 1], "|") == 0)) {
                validRedirect = 0;
                break;
            }

            // Redirect token checking
            if (i == tokenLength - 2) continue;
            if (strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], "<") == 0) {
                validRedirect = 0;
                break;
            }
        }

        if (!validRedirect) {
            printf("Invalid i/o redirection\n");
            continue;
        }

        /* Handle built-in commands */
        if (strcmp(tokens[0], "exit") == 0) {
            freeTokens(tokens);
            break;

        } else if (strcmp(tokens[0], "h") == 0 || strcmp(tokens[0], "history") == 0) {
            showCommandHistory();
            addToCommandHistory(line);

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
            // Retokenise by the pipe character '|' (note, all pipe characters will be split, not just " | "
            char **pipeTokens = tokenise(line, "|");

            int pipeTokensLength = 0;
            while (pipeTokens[pipeTokensLength] != NULL)
                trim(pipeTokens[pipeTokensLength++]); // Count the number of tokens

            int nPipes = pipeTokensLength - 1;
            int (*pipes)[2] = malloc(nPipes * sizeof(int) * 2);

            // [IN,OUT], [IN,OUT], [IN,OUT], [IN,OUT]
            //      \_____/    \____/    |____/    \_____STDOUT

            // only need n-1 pipes

            // Explode the glob and expand everything first.
            char ***arrTokens = malloc(sizeof(char **) * pipeTokensLength);
            char **exec = malloc(sizeof(char *) * pipeTokensLength);

            int allProgramsFound = 1;
            for (int i = 0; i < pipeTokensLength; i++) {
                arrTokens[i] = tokenise(pipeTokens[i], " ");
                if (strContains(pipeTokens[i], "*?[~"))
                    arrTokens[i] = fileNameExpand(arrTokens[i]); // Resolve wildcards

                // Whilst we're at it, confirm the paths exist
                if (!(exec[i] = findExecutable(arrTokens[i][0], path))) {
                    printf("%s: Command not found\n", arrTokens[i][0]);
                    allProgramsFound = 0;
                }
            }

            if (allProgramsFound) {
                for (int i = 0; i < pipeTokensLength; i++) {
//                    if (i == 0 ) close(pipes[0][0])
                    char **cmd = arrTokens[i];

                    /* Handle programs */
                    int stdin = STDIN_FILENO;
                    int stdout = STDOUT_FILENO;

                    int cmdLength = 0;
                    while (cmd[++cmdLength] != NULL); // Count the number of tokens

                    /* Check for redirection */
                    if (cmdLength >= 3 && strContains(cmd[cmdLength - 2], "<>")) {
                        // stdin redirection
                        if (strcmp(cmd[cmdLength - 2], "<") == 0 &&
                            (stdin = open(cmd[cmdLength - 1], O_RDONLY)) == -1) {
                            perror("Input redirection");
                            continue;
                        };

                        // stdout redirection
                        if (strcmp(cmd[cmdLength - 2], ">") == 0 &&
                            (stdout = open(cmd[cmdLength - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1) {
                            perror("Output redirection");
                            continue;
                        }

                        // Remove the redirect tokens as we don't want them to be passed to the program
                        free(cmd[cmdLength - 1]);
                        free(cmd[cmdLength - 2]);
                        cmd[cmdLength - 2] = NULL;
                        cmdLength -= 2;
                    }

                    if (i == 0) {
                        printf("Running ");
                        if (nPipes) {
                            for (char ***toks = arrTokens; toks < arrTokens + pipeTokensLength; toks++) {
                                if (toks != arrTokens) printf(" | ");

                                for (char **toks2 = *toks; *toks2; toks2++) {
                                    if (toks2 != *toks) printf(" ");
                                    printf("%s", *toks2);
                                }
                            }
                        } else {
                            printf("%s", exec[i]);
                        }
                        printf(" ...\n--------------------\n");
                    }

//printf("i is %d, nPipes is %d\n", i, nPipes);
                    if (i < nPipes) {
                        printf("CREATING PIPE [%d]\n", i);
                        if (pipe2(pipes[i], O_CLOEXEC ) != 0) {
                            perror(NULL);
                        };
                    }
                    if ((pid = fork() != 0)) {
                        /* Parent Process */
                        // Wait for child to exit, then print return code and add the command to the command history
                        // Close stdin/stdout redirector file descriptors if needed

                        if (i>0) {
printf("Close pipe %dW\n", i-1); close(pipes[i - 1][1]);
printf("Close pipe %dR\n", i-1); close(pipes[i - 1][0]);
                        }

                        wait(&stat);

                        if (stdin != STDIN_FILENO) close(stdin);
                        if (stdout != STDOUT_FILENO) close(stdout);
                        if (i == pipeTokensLength - 1) {
                            printf("--------------------\n");
                            printf("Returns %d\n", WEXITSTATUS(stat));
                            addToCommandHistory(line);
                        }
                    } else {
                        /* Child Process */
                        // Redirect stdin and stdout if needed and transform process
//                        printf("Child %d: %s\n", i, exec[i]);

//                        printf("`stdin` = %d\n", stdin);
//                        printf("`stdout` = %d\n", stdout);
//                        if (i > 0) printf("`pipes[%d]` = [%d, %d]\n", i - 1, pipes[i - 1][0], pipes[i - 1][1]);
//                        printf("`pipes[%d]` = [%d, %d] \n\n", i, pipes[i][0], pipes[i][1]);

                        if (nPipes) {
                            if (i != 0) {
//                                printf("Reading from fd %d\n", pipes[i - 1][0]);
                                dup2(pipes[i - 1][0], STDIN_FILENO);
                            } else {
//                                printf("Reading from STDIN\n");
                            }

                            if (i < nPipes) {
//                                printf("Writing to fd %d\n", pipes[i][1]);
                                dup2(pipes[i][1], STDOUT_FILENO);
                            } else {
//                                printf("Writing to STDOUT\n");
                            }
                        }

                        if (stdin != STDIN_FILENO) dup2(stdin, STDIN_FILENO);
                        if (stdout != STDOUT_FILENO) dup2(stdout, STDOUT_FILENO);

                        execve(exec[i], arrTokens[i], envp);
                        return -1;
                    }
                }
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
