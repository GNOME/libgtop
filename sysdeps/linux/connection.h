#ifndef __CONNECTION_H_
#define __CONNECTION_H_

#include <glib.h>
#include "packet.h"
#include <sys/types.h>

G_BEGIN_DECLS

typedef struct _Packet_list_node _Packet_list_node;
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
	PacketList *sent_packets;
	PacketList *received_packets;
	//ref packet is used to check if the packet belongs to the given connection
	Packet *ref_packet;
	//connection stats
	u_int64_t bytes_sent;
	u_int64_t byes_recv;
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
Connection *findConnection(Packet *pkt);
void Conn_list_init(Conn_list *clist, Connection *conn_val, Conn_list *next_val = NULL);
Connection *Conn_list_get_connection(Conn_list *clist);
void setNext(Conn_list *clist, Conn_list *next_val);
ConnList *Connection_list_get_next(Conn_list *clist);
int Connection_get_last_packet_time(Connection *conn);
//function to sum up data stats of old packets and delete them in a given connection
G_END_DECLS

#endif