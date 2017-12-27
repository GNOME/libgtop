#include <glib.h>
#include <sys/types.h>
#include <stdio.h>
#include "connection.h"
#include "process.h"

//Net_process functions
void 
Net_process_init(Net_process *proc, unsigned long inode_val, char *device_name_val, char *proc_name_val
{
	if (proc_name == NULL)
		proc->proc_name = proc_name_val;
	else
		proc_name = proc_name_val;
	//debug statement
	printf("Process name :%s\n",proc->proc_name);
	proc->device_name = device_name_val;
	//proc->inode_val = 
	proc->pid = 0;
	proc->uid = 0;
	proc->bytes_sent = 0;
	proc->bytes_recv = 0;
}

int 
Net_process_get_last_packet_time(Net_process *proc)
{
	int last_packet = 0;
	Conn_list *curr_conn = proc->connections;
	while (curr_conn != NULL)
	{
		g_assert(Conn_list_get_connection(curr_conn) != NULL)
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
	Conn_list *curr_conn = proc->connections;
	while (curr_conn != NULL)
	{
		Connection *conn = Conn_list_get_connection(curr_conn));
		sum_recv += conn->bytes_recv;
		sum_sent += conn->bytes_sent;
		curr_conn = Connection_list_get_next(curr_conn);
	}
	//debug
	printf("sent: %d recv: %d\n",sum_sent, sum_recv);
	*recvd = sum_recv + proc->bytes_recv; //check
	*sent = sum_sent + proc->bytes_sent; //check
}

float
to_kb(u_int64_t bytes)
{
	return ((double)bytes)/1024;
}

float
to_kbps(u_int64_t bytes)
{
	return to_kb(bytes)/PERIOD; //define macro
}
void
Net_process_get_kbps(Net_process *proc, float *recvd, float *sent, timeval currtime)
{
	u_int64_t sum_sent = 0;
	u_int64_t sum_recv = 0;
	Conn_list *curr_conn = proc->connections;
	Conn_list *previous = NULL;
	while (curr_conn != NULL)
	{
		if (Connection_get_last_packet_time(Conn_list_get_connection(curr_conn)) <= currtime.tv_sec - CONNTIMEOUT/*macro*/)
		{	//sum up bytes transfer for all connections in a connection list
			proc->bytes_sent = Conn_list_get_connection(curr_conn)->bytes_sent;
			proc->bytes_recv = Conn_list_get_connection(curr_conn)->bytes_recv;
			
			Conn_list *to_delete_list = curr_conn;
			Connection *conn_to_delete = Conn_list_get_connection(curr_conn);
			curr_conn = Connection_list_get_next(curr_conn);
			//changing the reference of the conn_list because the previous will be freed
			if (to_delete_list == proc->connections)
				proc->connections = curr_conn;
			//g_slice_new is used to allocate mem to these structs
			g_slice_free(Conn_list, to_delete_list);
			g_slice_free(Connection, conn_to_delete);
		}
		else
		{
			/*
			*sum and delete funct for connections in a connection list
			*
			**/
		}
	}
	*recvd = to_kbps(sum_recv);
	*sent = to_kbps(sum_sent);

}
uid_t Net_process_get_uid(Net_process *proc);
void Net_process_set_uid(Net_process *proc);
unsigned long Net_process_get_inode(Net_process *proc);

//Net_process_list function
void Net_process_list_init(Net_process_list *plist, Net_process *proc, Net_process_list *next_val);
