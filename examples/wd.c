#include <glibtop.h>
#include <glibtop/procwd.h>

#include <glib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	pid_t pid;
	glibtop_proc_wd buf;
	char **dirs, **dir;

	if (argc < 2 || !(pid = strtoul(argv[1], NULL, 0)))
		pid = getpid();

	glibtop_init();

	dirs = glibtop_get_proc_wd(&buf, pid);

	g_print("Process %u:\n"
		" - root: '%s'\n"
		" - exe: '%s'\n"
		" - working directories:\n",
		(unsigned)pid, buf.root, buf.exe);

	for (dir = dirs; *dir; ++dir)
		g_print("   - '%s'\n", *dir);

	g_strfreev(dirs);

	glibtop_close();

	return 0;
}

