#include <glibtop/dev_handles.h>
#include <glib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <glibtop/net_process.h>
#include <glibtop/stats.h>
#include <glibtop/netsockets.h>
#include <stdio.h>

int main()
{
	GMainLoop *loop;
    loop = g_main_loop_new ( NULL , FALSE );

	get_capture_status(TRUE);
	init_setup();

	g_timeout_add(1000,glibtop_init_packet_capture,NULL);
	g_main_loop_run (loop);
    g_main_loop_unref(loop);

	return 0;
}
