#ifndef __GLIBTOP_CONNECTION_H__
#define __GLIBTOP_CONNECTION_H__

#include <glib.h>
#include <glibtop/packet.h>
#include <sys/types.h>
G_BEGIN_DECLS

#define PERIOD 5
typedef struct _Packet_list_node Packet_list_node;
struct _Packet_list_node
{
	Packet *pkt;
	Packet_list_node *next;
}; 
//NOTE: packet list is created just to hold the first ele of the Packet_list_node linked list
typedef struct _Packet_list Packet_list;
struct _Packet_list
{
	Packet_list_node *content;
};

typedef struct _Connection Connection;
struct _Connection
{
	Packet_list *sent_packets;
	Packet_list *received_packets;
	//ref packet is used to check if the packet belongs to the given connection
	Packet *ref_packet;
	//connection stats
	u_int64_t bytes_sent;
	u_int64_t bytes_recv;
	//time when last packet was captured in this connection 
	int last_packet_time;
};

typedef struct _Conn_list Conn_list;
struct _Conn_list
{
	Connection *conn;
	Conn_list *next;
};
void Connection_init(Connection *conn, Packet *pkt);
void add_packet_to_connection(Connection *conn, Packet *pkt);
Connection *find_connection(Packet *pkt);
void Conn_list_init(Conn_list *clist, Connection *conn_val, Conn_list *next_val);
Connection *Conn_list_get_connection(Conn_list *clist);
void setNext(Conn_list *clist, Conn_list *next_val);
Conn_list *Connection_list_get_next(Conn_list *clist);
int Connection_get_last_packet_time(Connection *conn);
void Connection_sum_and_del(Connection *conn, struct timeval t, u_int64_t *recv, u_int64_t *sent);
void Connection_list_setNext(Conn_list *clist,Conn_list *next_val);
void Packet_list_init_beg(Packet_list *pktlist);
void Packet_list_init(Packet_list *pktlist, Packet *pkt_val);
Conn_list *get_global_connections_instance(Conn_list *val);

G_END_DECLS

#endif
