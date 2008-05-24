#include <glibtop.h>
#include <glibtop/procaffinity.h>

#include <glib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	pid_t pid;
	glibtop_proc_affinity buf;
	guint16 *cpus;
	size_t i;

	if (argc < 2 || !(pid = strtoul(argv[1], NULL, 0)))
		pid = getpid();

	glibtop_init();

	cpus = glibtop_get_proc_affinity(&buf, pid);

	g_print("Process %u:\n"
		" - all: %d\n",
		(unsigned)pid, buf.all);

	for (i = 0; i != buf.number; ++i)
		g_print(" - CPU#%u is set\n", cpus[i]);

	g_free(cpus);

	glibtop_close();

	return 0;
}

