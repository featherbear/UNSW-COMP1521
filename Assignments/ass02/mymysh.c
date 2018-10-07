// mysh.c ... a small shell
// Started by John Shepherd, September 2018

// Completed by Andrew Wong (z5206677), September/October 2018
// https://github.com/featherbear/UNSW-COMP1521/tree/master/Assignments/ass02

#define _GNU_SOURCE // Need this to make pipe2() work

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
// extern char *strdup(char *);

// Function forward references
void trim(char *);
int strContains(char *, char *);
char **tokenise(char *, char *);
char **fileNameExpand(char **);
void freeTokens(char **);
char *findExecutable(char *, char **);
int isExecutable(char *);
void prompt(void);

void printPWD();
int isValidRedirect(char **, int);

// Globals
#define MAXLINE 200

#define TRUE  1
#define FALSE 0

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
    // - use contents of ~/.mymysh_history file if it exists
    initCommandHistory();

    char line[MAXLINE];      // Line buffer to hold the input
    char **lineTokens;       // Array of strings to hold the tokenised line

    int tokenised = FALSE;   // Flag to freeTokens(lineTokens) on next loop

    // main loop: print prompt, read line, execute command
    for (; prompt(), fgets(line, MAXLINE, stdin) != NULL;
           tokenised = (tokenised && (freeTokens(lineTokens), 1)) && FALSE) {

        // For each loop, print out the prompt and request input
        // At the end of each loop, set `lineTokensLength` and `tokenised` to FALSE.
        //   Additionally, call `freeTokens(lineTokens)` if `tokenised` was TRUE.
        //   (tokenised && (..., 1)) is a hack to use the void function `freeTokens` in an expression
        //   ... tbh this is all a hack.

        trim(line);                  // remove leading/trailing spaces
        if (*line == '\0') continue; // An empty line was entered, ignore

        // Command history recall
        if (line[0] == '!') {

            // Validate syntax
            // - !! or !n
            if (line[1] != '!') {
                if (line[1] == '\0' || line[1] < '0' || line[1] > '9') {
                    // Show error if the given string is not valid
                    printf("Invalid history substitution\n");
                    continue;
                }
            }

            char *recallCmd;
            int cmdNo = 1;

            // If `!!` -> getLastCommandFromHistory()
            // Else `!n` -> getCommandFromHistory(n)
            recallCmd = (line[1] == '!' && line[2] == '\0') ? getLastCommandFromHistory()
                                                            : getCommandFromHistory((cmdNo = atoi(&line[1])));

            // Check if the command was found
            if (recallCmd) {
                // Command found, so replace contents of `line`.
                strcpy(line, recallCmd);
                printf("%s\n", line);
            } else {
                printf("No command #%d\n", cmdNo);
                continue;
            }
        }

        /* Tokeniser */

        // This tokenisation operation is only used for builtins and syntax validation
        // It is called again for each piped command
        lineTokens = tokenise(line, " \t"); // Tokenise input line
        if (strContains(line, "*?[~")) lineTokens = fileNameExpand(lineTokens); // Resolve wildcards
        tokenised = 1; // Flag for freeTokens()

        int lineTokensLength = 0;         // Count the number elements in `lineTokens`
        while (lineTokens[++lineTokensLength] != NULL);

        // Validate pipe and redirect tokens
        if (!isValidRedirect(lineTokens, lineTokensLength)) {
            printf("Invalid i/o redirection\n");
            continue;
        }

        /* Handle built-in commands */
        if (strcmp(lineTokens[0], "exit") == 0) {
            // Free tokens then exit
            freeTokens(lineTokens);
            break;
        } else if (strcmp(lineTokens[0], "h") == 0 || strcmp(lineTokens[0], "history") == 0) {
            // Show the command history
            showCommandHistory(stdout);
            addToCommandHistory(line);
        } else if (strcmp(lineTokens[0], "pwd") == 0) {
            // Show the current working directory
            printPWD();
            addToCommandHistory(line);
        } else if (strcmp(lineTokens[0], "cd") == 0) {
            // Change directory into `lineTokens[1]`, else the user's home directory
            if (chdir(lineTokens[1] ? lineTokens[1] : getenv("HOME")) == 0) {
                addToCommandHistory(line);
                printPWD();
            } else {
                perror(lineTokens[1]);
            }
        } else {
            /*
             * User commands
             */

            // Retokenise by the pipe character '|' (note, all pipe characters will be split, not just " | "
            char **pipeTokens = tokenise(line, "|");

            // Count the number of pipe character delimited tokens, also trimming each string to account for spaces
            int pipeTokensLength = 0;
            while (pipeTokens[pipeTokensLength] != NULL) trim(pipeTokens[pipeTokensLength++]);

            // For `n` piped commands, we need `n-1` pipes
            int nPipes = pipeTokensLength - 1;

            // Allocate memory for `nPipes` amount of two-width integer arrays
            int (*pipes)[2] = malloc(nPipes * sizeof(int) * 2);

            /* VISUALISATION
             *
             *      A         B         C         D
             *
             *   [IN,OUT], [IN,OUT], [IN,OUT], [IN,OUT]
             *         \___/     \___/     |___/     \_____STDOUT
             *
             *           a         b         c
             *
             *
             * Here we have four commands A B C D
             * We only need three pipes a b c
             *
             */

            /* Explode the glob and expand everything first */

            // Allocate memory for an array of an array of tokens
            char ***arrTokens = malloc(sizeof(char **) * pipeTokensLength);

            // Allocate memory for an array of path resolved executables
            char **exec = malloc(sizeof(char *) * pipeTokensLength);
            int allProgramsFound = 1; // Flag to check if all executables were path resolved successfully

            for (int i = 0; i < pipeTokensLength; i++) {
                // Tokenise each separate command
                arrTokens[i] = tokenise(pipeTokens[i], " ");

                // Resolve wildcards if they exist
                if (strContains(pipeTokens[i], "*?[~")) arrTokens[i] = fileNameExpand(arrTokens[i]);

                // Check that the executable exists
                if (!(exec[i] = findExecutable(arrTokens[i][0], path))) {
                    printf("%s: Command not found\n", arrTokens[i][0]);
                    allProgramsFound = 0;
                }
            }

            if (allProgramsFound) {
                /* Handle programs */
                for (int i = 0; i < pipeTokensLength; i++) {
                    char **cmd = arrTokens[i];

                    int redirIn  = STDIN_FILENO;
                    int redirOut = STDOUT_FILENO;

                    // Count the number of tokens in the command
                    int cmdLength = 0;
                    while (cmd[++cmdLength] != NULL);


                    /* Check for redirection on the last process */
                    if (i == pipeTokensLength - 1 && cmdLength >= 3) {
                        int redirectSuccess = FALSE;

                        // stdin redirection
                        if (strcmp(cmd[cmdLength - 2], "<") == 0) {
                            if ((redirIn = open(cmd[cmdLength - 1], O_RDONLY)) == -1) {
                                perror("Input redirection");
                                break;
                            }
                            redirectSuccess = TRUE;
                        };


                        // stdout redirection
                        if (strcmp(cmd[cmdLength - 2], ">") == 0) {
                            if ((redirOut = open(cmd[cmdLength - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1) {
                                perror("Output redirection");
                                break;
                            }
                            redirectSuccess = TRUE;
                        }

                        if (redirectSuccess) {
                            // Remove the redirect tokens as we don't want them to be passed to the program
                            free(cmd[cmdLength - 1]);
                            free(cmd[cmdLength - 2]);
                            cmd[cmdLength - 2] = NULL;
                            cmdLength -= 2;
                        }
                    }


                    // Print the command to run
                    if (i == 0) {
                        printf("Running ");

                        // If there are piped processes, print the entire command (with wildcards expanded)
                        if (nPipes) {
                            for (char ***toks = arrTokens; toks < arrTokens + pipeTokensLength; toks++) {
                                if (toks != arrTokens) printf(" | ");

                                for (char **toks2 = *toks; *toks2; toks2++) {
                                    if (toks2 != *toks) printf(" ");
                                    printf("%s", *toks2);
                                }
                            }
                        } else {
                            // Else print just the program name
                            printf("%s", exec[i]);
                        }
                        printf(" ...\n--------------------\n");
                    }



                    // Open n-1 pipes
                    if (i < nPipes && pipe2(pipes[i], O_CLOEXEC) != 0) perror("Pipe");

                    if ((pid = fork() != 0)) {
                        /* Parent Process */

                        // Close the previous pipe
                        if (i > 0) {
                            close(pipes[i - 1][1]);
                            close(pipes[i - 1][0]);
                        }

                        wait(&stat);

                        // Close stdin/stdout redirector file descriptors if needed
                        if (redirIn != STDIN_FILENO) close(redirIn);
                        if (redirOut != STDOUT_FILENO) close(redirOut);

                        // Print return code of the final command and add the original line to the command history
                        if (i == pipeTokensLength - 1) {
                            printf("--------------------\n");
                            printf("Returns %d\n", WEXITSTATUS(stat));
                            addToCommandHistory(line);
                            free(exec[i]);
                        }
                    } else {
                        /* Child Process */
                        // Redirect stdin and stdout if needed and transform process

                        if (nPipes) {
                            // Programs take the output of the previous program as their input
                            // - The first program doesn't take any input
                            // - The last program writes to STDOUT, or a specified redirected file
                            if (i != 0) dup2(pipes[i - 1][0], STDIN_FILENO);
                            if (i < nPipes) dup2(pipes[i][1], STDOUT_FILENO);
                        }

                        // Redirect input/output if redirect file specified
                        // - Redirecting the input of a file in a piped command won't work... eh.
                        if (redirIn  != STDIN_FILENO)  dup2(redirIn, STDIN_FILENO);
                        if (redirOut != STDOUT_FILENO) dup2(redirOut, STDOUT_FILENO);

                        execve(exec[i], arrTokens[i], envp);

                        // Print to stdout instead of stderr to follow jas' version
                        fprintf(stdout, "%s: unknown type of executable\n", exec[i]);
                        return -1;
                    }
                    freeTokens(arrTokens[i]);
                }
            }
            // Clean up
            free(pipes);
            free(exec);
            free(arrTokens);
            freeTokens(pipeTokens);
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

    // NULL-terminate the token array
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

/* My additional functions */


// printPWD: print the current working directory
void printPWD() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        printf("Can't find working directory!!\n");
    }
}

// isValidRedirect: validate pipe and redirect tokens
// "|" token cannot be be adjacent to another pipe token
// "|" token cannot exist as the first or last token
// ">" or "<" (mutually exclusive) can only exist as the second last token
int isValidRedirect(char** lineTokens, int lineTokensLength) {
    // Check if first or last token is a pipe
    if (strcmp(lineTokens[0], "|") == 0 ||
        (lineTokensLength > 1 && strcmp(lineTokens[lineTokensLength - 1], "|") == 0)) {
        return FALSE;
    } else {
        for (int i = 0; i < lineTokensLength; i++) {
            // Sequential pipe check
            if (i > 0 && strcmp(lineTokens[i], "|") == 0 && strcmp(lineTokens[i - 1], "|") == 0) {
                return FALSE;
            }

            // Redirect '>'/'<' checking (can only be in the second last position)
            if (i == lineTokensLength - 2) continue;
            if (strcmp(lineTokens[i], ">") == 0 || strcmp(lineTokens[i], "<") == 0) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
