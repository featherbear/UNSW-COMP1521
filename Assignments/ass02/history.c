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

// I got rid of HistoryEntry!!!

/*
 * So, it's pretty important to explain what's happening I guess...
 * This implementation keeps only the last 20 commands, so the recall only goes from !1 - !HISTSIZE   (1 based)
 * This is achieved by using a modulo `n % m` which will give a number between  -(m-1) and (m-1)
 * We want to get a number between 0 and m-1 so we can modify the expression to `(n + m) % m`
 *
 * This makes the numbering cyclical... 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 ... (Where m = 10)
 */

typedef struct _history_list {
    int nEntries;
    char commands[MAXHIST][MAXSTR];
    int position;
} HistoryList;

HistoryList CommandHistory;
char *HISTPATH;

// strip: remove leading/trailing spaces and newline characters from a string
// 'inspired' by trim() from mymysh.c :)
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
    // Initialise the values of `CommandHistory`
    CommandHistory.nEntries = 0;
    CommandHistory.position = 0;

    // HISTPATH = $HOME/HISTFILE
    char *HOME = getenv("HOME");
    HISTPATH = malloc(strlen(HOME) + 1 + strlen(HISTFILE) + 1);
    sprintf(HISTPATH, "%s/%s", HOME, HISTFILE);

    // Read each line in the file
    FILE *file;
    if ((file = fopen(HISTPATH, "r"))) {
        char cmd[MAXSTR];
        while (fgets(cmd, MAXSTR, file)) {
            strip(cmd);
            addToCommandHistory(cmd);
        }
        fclose(file);
        return CommandHistory.position;
    }
    return -1;
}

// addToCommandHistory()
// - add a command line to the history list
// - overwrite oldest entry if buffer is full
void addToCommandHistory(char *cmdLine) {
    int i = CommandHistory.position;

    // If current entry is empty, increase the entry count
    if (CommandHistory.commands[i][0] == '\0') CommandHistory.nEntries++;

    // Copy the command into the current entry
    strcpy(CommandHistory.commands[i], cmdLine);

    // Update current position index
    CommandHistory.position = (i + 1) % MAXHIST;
}

// showCommandHistory()
// - display the list of commands in the history
void showCommandHistory(void) {
    // Get the first entry
    int offsetOne = CommandHistory.position - CommandHistory.nEntries;

    // Print out each past command
    for (int i = 0; i < CommandHistory.nEntries; i++) {
        int j = (offsetOne + i + MAXHIST) % MAXHIST; // Get index from 0 - MAXHIST-1
        printf(" %3d %s\n", i + 1, CommandHistory.commands[j]);
    }
}

// getCommandFromHistory()
// - get the command line for specified command
// - returns NULL if no command with this number
char *getCommandFromHistory(int cmdNo) {
    // Check for indexes that are out of range
    if (cmdNo < 0 || cmdNo >= MAXHIST) return NULL;

    // Get index from 0 - MAXHIST-1
    int offsetOne = CommandHistory.position - CommandHistory.nEntries;
    int j = (offsetOne + cmdNo + MAXHIST) % MAXHIST;

    // Return the command if an entry exists
    if (CommandHistory.commands[j][0] != '\0') return CommandHistory.commands[j];

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
    // Get the first entry
    int offsetOne = CommandHistory.position - CommandHistory.nEntries;

    // Write each command, starting from the first entry
    FILE *file;
    if ((file = fopen(HISTPATH, "w"))) {
        for (int i = 0; i < CommandHistory.nEntries; i++) {
            int j = (offsetOne + i + MAXHIST) % MAXHIST;
            fprintf(file, "%s\n", CommandHistory.commands[j]);
        }
        fclose(file);
    }
}

// cleanCommandHistory
// - release all data allocated to command history
void cleanCommandHistory() {
    free(HISTPATH);
}
