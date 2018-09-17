// COMP1521 18s2 mysh ... command history
// Implements an abstract data object

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

// This is defined in string.h
// BUT ONLY if you use -std=gnu99
//extern char *strdup(const char *s);

// Command History
// array of command lines
// each is associated with a sequence number

#define MAXHIST 20
#define MAXSTR  200

#define HISTFILE ".mymysh_history"

typedef struct _history_list {
    int nEntries;
    char commands[MAXHIST][MAXSTR];
    int position;
} HistoryList;

HistoryList CommandHistory;

FILE *file;
char *HISTPATH;
int MAXHIST_STRLEN;
char *SHOW_FORMAT;

// strip: remove leading/trailing spaces and newline characters from a string
void strip(char *str) {
    int first, last;
    first = 0;
    while (str[first] == ' ' || str[first] == '\n') first++;
    last = strlen(str) - 1;
    while (str[last] == ' ' || str[last] == '\n') last--;
    int i, j = 0;
    for (i = first; i <= last; i++) str[j++] = str[i];
    str[j] = '\0';
}

// initCommandHistory()
// - initialise the data structure
// - read from HISTFILE if it exists

int initCommandHistory() {

    MAXHIST_STRLEN = 0;
    for (int n = MAXHIST; n; n /= 10, MAXHIST_STRLEN++);
    SHOW_FORMAT = malloc(10 + MAXHIST_STRLEN);
    sprintf(SHOW_FORMAT, "  %%%dd  %%s\n", MAXHIST_STRLEN);

    CommandHistory.nEntries = 0;
    CommandHistory.position = 0;

    char *HOME = getenv("HOME");
    HISTPATH = malloc(strlen(HOME) + 1 + strlen(HISTFILE) + 1);
    sprintf(HISTPATH, "%s/%s", HOME, HISTFILE);

    if ((file = fopen(HISTPATH, "r"))) {
        char cmd[MAXSTR];
        while (fgets(cmd, MAXSTR, file)) {
            strip(cmd);
            addToCommandHistory(cmd);
        }

        fclose(file);
        return 0;
    }
    return -1;
}

// addToCommandHistory()
// - add a command line to the history list
// - overwrite oldest entry if buffer is full

void addToCommandHistory(char *cmdLine) {
    // cmdLine is at most 200 chars long
    int i = CommandHistory.position;

    if (*(CommandHistory.commands[i]) != '\0') CommandHistory.nEntries--;
    strcpy(CommandHistory.commands[i], cmdLine);

    CommandHistory.nEntries++;

    CommandHistory.position = (i + 1) % MAXHIST;
}

// showCommandHistory()
// - display the list of commands in the history

void showCommandHistory(void) {

    int offsetOne = CommandHistory.position - CommandHistory.nEntries;
    for (int i = 0; i < CommandHistory.nEntries; i++) {
        int j = (offsetOne + i + MAXHIST) % MAXHIST;
        printf(SHOW_FORMAT, i + 1, CommandHistory.commands[j]);
    }
    // TODO show one ahead
}

// getCommandFromHistory()
// - get the command line for specified command
// - returns NULL if no command with this number

char *getCommandFromHistory(int cmdNo) {
    if (cmdNo < 0 || cmdNo >= MAXHIST) return NULL;

    int offsetOne = CommandHistory.position - CommandHistory.nEntries;
    int j = (offsetOne + cmdNo + MAXHIST) % MAXHIST;
    if (CommandHistory.commands[j] != '\0') return CommandHistory.commands[j];

    return NULL;
}

// getLastCommandFromHistory()
// - get the last command line
// - returns NULL if no previous command

char *getLastCommandFromHistory() {
    return getCommandFromHistory((CommandHistory.position - 1 + MAXHIST) % MAXHIST);
}

// saveCommandHistory()
// - write history to $HOME/.mymysh_history

void saveCommandHistory() {
    int offsetOne = CommandHistory.position - CommandHistory.nEntries;

    if ((file = fopen(HISTPATH, "w"))) {
        for (int i = 0; i < CommandHistory.nEntries; i++) {
            int j = (offsetOne + i + MAXHIST) % MAXHIST;
            fputs(CommandHistory.commands[j], file);
            fputc('\n', file);
        }
    }
}

// cleanCommandHistory
// - release all data allocated to command history

void cleanCommandHistory() {
    // TODO
    free(SHOW_FORMAT);
    free(HISTPATH);

}
