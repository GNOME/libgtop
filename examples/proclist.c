#include <glibtop.h>
#include <glibtop/proclist.h>

#include <glib.h>

#include <stdio.h>

int main()
{
	unsigned *pids;
	unsigned i;
	glibtop_proclist buf;

	glibtop_init();

	pids = glibtop_get_proclist(&buf, 0, 0);

	for (i = 0; i < buf.number; ++i)
		printf("%u ", pids[i]);

	putchar('\n');

	g_free(pids);

	glibtop_close();

	return 0;
}
