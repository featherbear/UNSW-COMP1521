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
#define HISTFORMATTER " %3d  %s\n"

typedef struct _history_entry {
    int seqNumber;
    char commandLine[MAXSTR];
} HistoryEntry;

typedef struct _history_list {
    int nEntries;
    HistoryEntry commands[MAXHIST];
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
    CommandHistory.position = 1;

    // HISTPATH = $HOME/HISTFILE
    char *HOME = getenv("HOME");
    HISTPATH = malloc(strlen(HOME) + 1 + strlen(HISTFILE) + 1);
    sprintf(HISTPATH, "%s/%s", HOME, HISTFILE);

    // Initialise history entries
    for (int i = 0; i < MAXHIST; i++) {
        CommandHistory.commands[i].seqNumber = -1;
    }

    // Read each line in the file
    FILE *file;
    if ((file = fopen(HISTPATH, "r"))) {
        char line[MAXSTR];
        while (fgets(line, MAXSTR, file)) {
            // strip(line);
            char cmd[MAXSTR];
            sscanf(line, "%d %[^\n]", &(CommandHistory.position), cmd);
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
    int index = CommandHistory.position % MAXHIST;
    // If current entry is empty, increase the entry count
    if (CommandHistory.commands[index].seqNumber == -1) CommandHistory.nEntries++;

    // Copy the command into the current entry and update position
    strcpy(CommandHistory.commands[index].commandLine, cmdLine);
    CommandHistory.commands[index].seqNumber = CommandHistory.position++;
}

// showCommandHistory()
// - display the list of commands in the history
void showCommandHistory(void) {
    // Get the first entry
    int offsetOne = CommandHistory.position - CommandHistory.nEntries;

    // Print out each past command
    for (int i = 0; i < CommandHistory.nEntries; i++) {
        int j = (offsetOne + i + MAXHIST) % MAXHIST; // Get index from 0 - MAXHIST-1
        printf(HISTFORMATTER, CommandHistory.commands[j].seqNumber, CommandHistory.commands[j].commandLine);
    }
}

// getCommandFromHistory()
// - get the command line for specified command
// - returns NULL if no command with this number
char *getCommandFromHistory(int cmdNo) {
    // Check for indexes that are out of range
    int cmdMin = CommandHistory.position - CommandHistory.nEntries;
    int cmdMax = CommandHistory.position - 1;
    if (cmdNo < cmdMin || cmdNo > cmdMax) return NULL;

    // Get index from 0 - MAXHIST-1
    int offsetOne = CommandHistory.position - CommandHistory.nEntries;
    int j = (offsetOne + cmdNo - cmdMin + MAXHIST) % MAXHIST;

    // Return the command if an entry exists
    if (CommandHistory.commands[j].seqNumber != '\0') return CommandHistory.commands[j].commandLine;

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
            fprintf(file, HISTFORMATTER, CommandHistory.commands[j].seqNumber, CommandHistory.commands[j].commandLine);
        }
        fclose(file);
    }
}

// cleanCommandHistory
// - release all data allocated to command history
void cleanCommandHistory() {
    free(HISTPATH);
}
