#include <glib.h>
#include <sys/types.h>
#include <stdio.h>
#include <glibtop/connection.h>
#include <glibtop/net_process.h>

void 
Net_process_init(Net_process *proc, unsigned long pid , char *device_name_val, char *proc_name_val)
{
	proc->proc_name = proc_name_val;
	//debug statement
	printf("Process name :%s\n",proc->proc_name);
	proc->device_name = device_name_val;
	proc->pid = pid;
	proc->uid = 0;
	proc->bytes_sent = 0;
	proc->bytes_recv = 0;
	proc->proc_connections = NULL;
}

int 
Net_process_get_last_packet_time(Net_process *proc)
{
	int last_packet = 0;
	Conn_list *curr_conn = proc->proc_connections;
	while (curr_conn != NULL)
	{
		g_assert(Conn_list_get_connection(curr_conn) != NULL);
		if (Connection_get_last_packet_time(Conn_list_get_connection(curr_conn)) > last_packet)
			last_packet = Connection_get_last_packet_time(Conn_list_get_connection(curr_conn));
		curr_conn = Connection_list_get_next(curr_conn);
	}
	return last_packet;
}

void Net_process_get_total(Net_process *proc, u_int64_t *recvd, u_int64_t *sent)
{
	u_int64_t sum_sent = 0;
	u_int64_t sum_recv = 0;
	Conn_list *curr_conn = proc->proc_connections;
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

float
to_kb(u_int64_t bytes)
{
	return ((double)bytes)/1024;
}

float
to_kbps(u_int64_t bytes)
{
	return to_kb(bytes)/PERIOD;
}
void
Net_process_get_kbps(Net_process *proc, float *recvd, float *sent, struct timeval curtime)
{
	u_int64_t sum_sent = 0;
	u_int64_t sum_recv = 0;
	Conn_list *curr_conn = proc->proc_connections;
	Conn_list *previous = NULL;
	while (curr_conn != NULL)
	{
		u_int64_t sent_val = 0;
		u_int64_t recv = 0;
		Connection_sum_and_del(Conn_list_get_connection(curr_conn), curtime, &recv, &sent_val);
		sum_recv += recv;
		sum_sent += sent_val;
		previous = curr_conn;
		curr_conn = Connection_list_get_next(curr_conn); 
	}
	*recvd = to_kbps(sum_recv);
	*sent = to_kbps(sum_sent);

}

uid_t
Net_process_get_uid(Net_process *proc)
{
	return proc->uid;
}

void
Net_process_list_init(Net_process_list *plist, Net_process *proc, Net_process_list *next_val)
{
	plist->val = proc;
	plist->next = next_val;
}

Net_process *
Net_process_list_get_proc(Net_process_list *plist)
{
	return plist->val;
}

Net_process_list *
get_proc_list_instance(Net_process_list *val)
{
	static Net_process_list *temp_proc_list = NULL;
	if (val != NULL)
		temp_proc_list = val;
	else if (temp_proc_list == NULL)
		temp_proc_list = g_slice_new(Net_process_list);
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
