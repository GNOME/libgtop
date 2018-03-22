#include <glibtop/dev_handles.h>
#include <glib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <glibtop/net_process.h>
#include <glibtop/stats.h>
#include <glibtop/netsockets.h>
#include <stdio.h>

static void
network_stats_print_entry(network_stats_entry *st)
{
	printf("pid :%d \t sent_value:%d \trecv value:%d \t \n", st->pid, st->sent_value, st->recv_value);
}

int main()
{
	GMainLoop *loop;
	loop = g_main_loop_new(NULL, FALSE );
	glibtop_get_capture_status(TRUE);
	glibtop_init_setup();
	g_timeout_add(1000,glibtop_init_packet_capture,NULL);
	g_main_loop_run(loop);
	g_main_loop_unref(loop);

	return 0;
}
