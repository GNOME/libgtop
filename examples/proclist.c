#include <glibtop.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>

#include <glib.h>

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>


static void print_pids(guint64 which, guint64 arg)
{
	pid_t *pids;
	unsigned i;
	glibtop_proclist buf;

	pids = glibtop_get_proclist(&buf, which, arg);

	for (i = 0; i < buf.number; ++i) {
		glibtop_proc_time ptime;
		glibtop_proc_uid puid;
		glibtop_proc_state pstate;

		pid_t p = pids[i];

		glibtop_get_proc_time(&ptime, p);
		glibtop_get_proc_uid(&puid, p);
		glibtop_get_proc_state(&pstate, p);

		/* almost equivalent to ps -x -o user,pid,time,usertime,systime,start,command */
		printf("%u pid=%u real=%.2f user=%.2f sys=%.2f start=%lu %s (%lx)\n",
		       puid.uid,
		       p,
		       (double)ptime.rtime / ptime.frequency,
		       (double)ptime.utime / ptime.frequency,
		       (double)ptime.stime / ptime.frequency,
		       (unsigned long)ptime.start_time,
		       pstate.cmd,
		       (long)ptime.flags);
	}

	g_free(pids);
}


int main()
{
	glibtop_init();

	print_pids(GLIBTOP_KERN_PROC_UID, getuid());

	glibtop_close();

	return 0;
}
