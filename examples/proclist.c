#include <glibtop.h>
#include <glibtop/proclist.h>

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

	printf("glibtop_get_proclist(%#llx, %llu) -> %lu processes\n",
	       which, arg, (unsigned long)buf.number);

	for (i = 0; i < buf.number; ++i)
		printf("%u ", pids[i]);

	putchar('\n');
	putchar('\n');

	g_free(pids);
}


int main()
{
	glibtop_init();

	print_pids(GLIBTOP_KERN_PROC_ALL, 0);
	print_pids(GLIBTOP_KERN_PROC_UID, getuid());

	glibtop_close();

	return 0;
}
