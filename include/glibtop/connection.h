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
	guint64 bytes_sent;
	guint64 bytes_recv;
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
GSList *Conn_list_init(GSList *clist, Connection *conn_val);
Connection *Conn_list_get_connection(GSList *clist);
void setNext(Conn_list *clist, Conn_list *next_val);
GSList *Connection_list_get_next(GSList *clist);
int Connection_get_last_packet_time(Connection *conn);
void Connection_sum_and_del(Connection *conn, struct timeval t, guint64 *recv, guint64 *sent);
void Connection_list_setNext(Conn_list *clist,Conn_list *next_val);
void Packet_list_init_beg(Packet_list *pktlist);
void Packet_list_init(Packet_list *pktlist, Packet *pkt_val);
GSList *get_global_connections_instance(GSList *val);

G_END_DECLS

#endif
