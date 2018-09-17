// COMP1521 18s2 mymysh ... command history
// Implements an interface to an abstract data object

#include <stdio.h>

// Functions on the Command History object

int initCommandHistory();
void addToCommandHistory(char *cmdLine);
void showCommandHistory();
char *getCommandFromHistory(int cmdNo);
char *getLastCommandFromHistory();
void saveCommandHistory();
void cleanCommandHistory();
void strip(char *str);
