#include "proc/ps.h"
#include "proc/readproc.h"

char * status(proc_t* task) {
    static char buf[4] = "   ";

    buf[0] = task->state;
    if (task->rss == 0 && task->state != 'Z')
        buf[1] = 'W';
    else
        buf[1] = ' ';
    if (task->nice < 0)
	buf[2] = '<';
    else if (task->nice > 0)
	buf[2] = 'N';
    else
	buf[2] = ' ';

    return(buf);
}
