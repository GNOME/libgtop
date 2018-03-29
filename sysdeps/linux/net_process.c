#include <glib.h>
#include <sys/types.h>
#include <stdio.h>
#include <glibtop/connection.h>
#include <glibtop/net_process.h>

void
Net_process_init(Net_process *proc, unsigned long pid)
{
	proc->pid = pid;
	proc->bytes_sent = 0;
	proc->bytes_recv = 0;
	proc->proc_connections = NULL;
}

time_t
Net_process_get_last_packet_time(Net_process *proc)
{
	time_t last_packet = 0;
	GSList *curr_conn_list = proc->proc_connections;
	while (curr_conn_list != NULL)
	{
		Connection *curr_conn = Conn_list_get_connection(curr_conn_list);
		g_assert(curr_conn != NULL);
		if (Connection_get_last_packet_time(curr_conn) > last_packet)
			last_packet = Connection_get_last_packet_time(curr_conn);
		curr_conn_list = Connection_list_get_next(curr_conn_list);
	}
	return last_packet;
}

void Net_process_get_total(Net_process *proc, guint64 *recvd, guint64 *sent)
{
	guint64 sum_sent = 0;
	guint64 sum_recv = 0;
	GSList *curr_conn = proc->proc_connections;
	while (curr_conn != NULL)
	{
		Connection *conn = Conn_list_get_connection(curr_conn);
		sum_recv += conn->bytes_recv;
		sum_sent += conn->bytes_sent;
		curr_conn = Connection_list_get_next(curr_conn);
	}
	*recvd = sum_recv + proc->bytes_recv;
	*sent = sum_sent + proc->bytes_sent;
}

void
Net_process_get_bytes(Net_process *proc, guint *recvd, guint *sent, struct timeval curtime)
{
	guint64 sum_sent = 0;
	guint64 sum_recv = 0;
	GSList *curr_conn = proc->proc_connections;
	GSList *previous = NULL;
	while (curr_conn != NULL)
	{
		gint64 sent_val = 0;
		guint64 recv = 0;
		Connection_sum_and_del(Conn_list_get_connection(curr_conn), curtime, &recv, &sent_val);
		sum_recv += recv;
		sum_sent += sent_val;
		previous = curr_conn;
		curr_conn = Connection_list_get_next(curr_conn);
	}
	*recvd = sum_recv;
	*sent = sum_sent;
}

GSList *
Net_process_list_init(GSList *net_proc_list, Net_process *proc)
{
	return g_slist_append (net_proc_list, proc);
}

Net_process *
Net_process_list_get_proc(GSList *plist)
{
	return plist->data;
}

GSList *
get_proc_list_instance(GSList *val)
{
	static GSList *temp_proc_list = NULL;
	if (val != NULL)
		temp_proc_list = val;
	return temp_proc_list;
}

Net_process *
get_unknown_proc_instance(Net_process *val)
{
	static Net_process *temp_proc = NULL;
	if (val != NULL)
		temp_proc = val;
	else if (temp_proc == NULL)
		temp_proc = g_slice_new(Net_process);
	return temp_proc;
}
