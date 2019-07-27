#ifndef __GLIBTOP_CONNECTION_H__
#define __GLIBTOP_CONNECTION_H__

#include <glib.h>
#include <glibtop/packet.h>
#include <sys/types.h>
G_BEGIN_DECLS

#define PERIOD 5

typedef struct _Connection Connection;
struct _Connection
{
	GSList *sent_packets;
	GSList *received_packets;
	//ref packet is used to check if the packet belongs to the given connection
	Packet *ref_packet;
	//connection stats
	guint64 bytes_sent;
	guint64 bytes_recv;
	//time when last packet was captured in this connection 
	time_t last_packet_time;
};

void Connection_init(Connection *conn, Packet *pkt);
void add_packet_to_connection(Connection *conn, Packet *pkt);
Connection *find_connection(Packet *pkt);
GSList *Conn_list_init(GSList *clist, Connection *conn_val);
Connection *Conn_list_get_connection(GSList *clist);
GSList *Connection_list_get_next(GSList *clist);
time_t Connection_get_last_packet_time(Connection *conn);
void Connection_sum_and_del(Connection *conn, struct timeval t, guint64 *recv, guint64 *sent);
GSList *Packet_list_init(GSList *pktlist, Packet *pkt_val);
GSList *get_global_connections_instance(GSList *val);

G_END_DECLS

#endif
