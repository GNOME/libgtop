
#include <sys/types.h>

dev_t name_to_dev(char* name);
char* dev_to_name(dev_t num);

dev_t tty_to_dev(char *tty);
void  dev_to_tty(char *tty, int dev);

char* abbrev_of_tty(char *tty);
