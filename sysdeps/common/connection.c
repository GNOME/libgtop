#include <glibtop/connection.h>
#include <glibtop/packet.h>
#include <stdio.h>

GSList *
Packet_list_init(GSList *pktlist, Packet *pkt_val)
{
	return g_slist_prepend(pktlist, pkt_val);
}

//adding a packet to the linked list
static void
addPacket(GSList **pktlist, Packet *pkt)
{
	if (*pktlist == NULL)
	{
		*pktlist = Packet_list_init(*pktlist, pkt);
		return;
	}
	if ((Packet *)((*pktlist)->data) != NULL)
	{
		if (((Packet *)((*pktlist)->data))->time.tv_sec == pkt->time.tv_sec)
		{
			((Packet *)((*pktlist)->data))->len += pkt->len;
			return;
		}
	}

	*pktlist = Packet_list_init(*pktlist, pkt);
}

//this packet to get the time of the last pcket capture for th given connection
void
add_packet_to_connection(Connection *conn, Packet *pkt)
{
	conn->last_packet_time = pkt->time.tv_sec;
	//if outgoing or incoming accordingly add the packet to respective packet list sent or recv i.e malloc a new 
	if (is_pkt_outgoing(pkt)) //check packet.c
	{
		conn->bytes_sent += pkt->len;
		addPacket(&(conn->sent_packets), pkt);
	}
	else
	{
		conn->bytes_recv += pkt->len;
		addPacket(&(conn->received_packets), pkt);
	}
}

GSList *
Conn_list_init(GSList *clist, Connection *conn_val)
{
	return g_slist_append(clist, conn_val);
}

Connection *
Conn_list_get_connection(GSList *clist)
{
	return clist->data;
}

GSList *
Connection_list_get_next(GSList *clist)
{
	return clist->next;
}

GSList *
get_global_connections_instance(GSList *val)
{
	static GSList *global_connections_list = NULL;
	if (val != NULL)
		global_connections_list = val;
	return global_connections_list;
}

void
Connection_init(Connection *conn, Packet *pkt)
{
	GSList *connections = get_global_connections_instance(NULL);
	GSList *temp = Conn_list_init(connections, conn);
	get_global_connections_instance(temp); //to set connections = temp in the static var global_connections_list
	conn->sent_packets = NULL;

	conn->received_packets = NULL;
	conn->bytes_sent = 0;
	conn->bytes_recv = 0;
	if (is_pkt_outgoing(pkt))
	{
		conn->bytes_sent += pkt->len;
		addPacket(&(conn->sent_packets), pkt);
		conn->ref_packet = Packet_init(pkt);
	}
	else
	{
		conn->bytes_recv += pkt->len;
		addPacket(&(conn->received_packets), pkt);
		conn->ref_packet = get_inverted_packet(pkt);
	}
	conn->last_packet_time = pkt->time.tv_sec;

}

static Connection *
find_connection_with_matching_source(Packet *pkt)
{
	GSList *current = get_global_connections_instance(NULL);
	while (current != NULL)
	{
		if (packet_match_source(pkt, Conn_list_get_connection(current)->ref_packet))
			return current->data;
		current = current->next;
	}
	return NULL;
}

static Connection *
find_connection_with_matching_ref_packet_or_source(Packet *pkt)
{
	GSList *current = get_global_connections_instance(NULL);
	while (current != NULL)
	{
		if (packet_match(pkt, Conn_list_get_connection(current)->ref_packet))
			return current->data;
		current = current->next;
	}
	return find_connection_with_matching_source(pkt);
}

Connection *
find_connection(Packet *pkt)
{
	if (is_pkt_outgoing(pkt))
		return find_connection_with_matching_ref_packet_or_source(pkt);
	else
	{
		Packet *inverted_packet = get_inverted_packet(pkt);
		Connection *result = find_connection_with_matching_ref_packet_or_source(inverted_packet);
		g_slice_free(Packet, inverted_packet);
		return result;
	}
}

time_t
Connection_get_last_packet_time(Connection *conn)
{
	return conn->last_packet_time;
}

static void
free_packets(gpointer data)
{
  Packet *temp = data;
  g_slice_free (Packet, temp);
}

static guint64
Packet_list_sum_and_del(GSList **pktlist, struct timeval t)
{
	guint64 sum = 0;
	GSList *current = *pktlist;
	GSList *previous = NULL;
	while (current != NULL && previous != current && current->data)
	{
		if (!(((Packet *)(current->data))->time.tv_sec <= t.tv_sec - PERIOD))
		{
			sum += ((Packet *)current->data)->len;
			previous = current;
			current = current->next;
		}
		else
		{
			if(current == *pktlist)
				*pktlist = NULL;
			else
			{
				previous->next = NULL;
				g_slist_free_full(current, free_packets);
			}
			return sum;
		}
	}
	return sum;
}

void
Connection_sum_and_del(Connection *conn, struct timeval t, guint64 *recv, guint64 *sent)
{
 	*sent = 0;
	*recv = 0;
	*sent = Packet_list_sum_and_del(&(conn->sent_packets), t);
	*recv = Packet_list_sum_and_del(&(conn->received_packets), t);
}
