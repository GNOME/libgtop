#include <glibtop.h>
#include <glibtop/mem.h>

#include <glib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	glibtop_mem buf;

	glibtop_init();

	g_print("%lx\n", glibtop_global_server->features);

	glibtop_get_mem(&buf);

	glibtop_close();

	return 0;
}

