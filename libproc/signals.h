/* signals.h - signal name handling */

void list_signals(void);

/* Lists all known signal names on standard output. */

int get_signal(char *name,char *cmd);
int get_signal2(char *name);

/* Returns the signal number of NAME. If no such signal exists, an error
   message is displayed and the program is terminated. CMD is the name of the
   application. */
