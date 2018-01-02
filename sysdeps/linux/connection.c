#include "connection.h"
#include "packet.h"
//global list of all connections being monitored
Conn_list *connections;
//initializer for packet list node
void
Packet_list_node_init(Packet_list_node *pktlist_node, Packet *pkt_val, Packet_list_node *next_val)
{
	pktlist_node->pkt = pkt_val;
	pktlist_node->next = next_val;
}

//initializers for packet list
void
Packet_list_init_beg(Packet_list *pktlist)
{
	pktlist->content = NULL;
}
void
Packet_list_init(Packet_list *pktlist, Packet *pkt_val)
{
	Packet_list_node *pktlistnode = g_slice_new(Packet_list_node);
	Packet_list_node_init(pktlistnode, pkt_val,NULL);
	pktlist->content = pktlistnode;
}

//adding a packet to the linked list
void addPacket(Packet_list *pktlist, Packet *pkt)
{
	if(pktlist->content == NULL)
	{	
		Packet *copy_pkt = g_slice_new(Packet);
		Packet_init(copy_pkt,*pkt);
		Packet_list_node *pktNode = g_slice_new(Packet_list_node);
		Packet_list_node_init(pktNode, copy_pkt, NULL);
		pktlist->content = pktNode;
		return;
	}
	if(pktlist->content->pkt->time.tv_sec == pkt->time.tv_sec)
	{
		pktlist->content->pkt->len += pkt->len;
		return;
	}

	//add a new node to the list and make this new node as the content of the Packet_list
	//currently a copy of the packet is made so that later we can delete the orginal capture
	Packet *copy_pkt = g_slice_new(Packet);
	Packet_init(copy_pkt,*pkt);
	Packet_list_node *pktNode = g_slice_new(Packet_list_node);
	Packet_list_node_init(pktNode, copy_pkt, pktlist->content);
	pktlist->content = pktNode;
}

//this packet to get the time of the last pcket capture for th given connection
void add_packet_to_connection(Connection *conn, Packet *pkt)
{
	conn->last_packet_time = pkt->time.tv_sec;
	//if outgoing or incoming accordingly add the packet to respective packet list sent or recv i.e malloc a new 
	if(is_pkt_outgoing(pkt)) //check packet.c
	{
		conn->bytes_sent += pkt->len;
		addPacket(conn->sent_packets,pkt);

	}
	else
	{
		conn->bytes_recv += pkt->len;
		addPacket(conn->received_packets,pkt);
	}
}

void
Conn_list_init(Conn_list *clist,Connection *conn_val, Conn_list *next_val)
{
	clist->conn = conn_val;
	clist->next = next_val;
}

Connection *
Conn_list_get_connection(Conn_list *clist)
{
	return clist->conn;
}

void 
Connection_list_setNext(Conn_list *clist,Conn_list *next_val)
{
	clist->next = next_val;
}

Conn_list *
Connection_list_get_next(Conn_list *clist)
{
	return clist->next;
}

void
Connection_init(Connection *conn, Packet *pkt)
{
	Conn_list *temp = g_slice_new(Conn_list);
	Conn_list_init(temp, conn,connections);
	connections = temp;
	conn->sent_packets = g_slice_new(Packet_list);
	Packet_list_init_beg(conn->sent_packets);
	conn->received_packets = g_slice_new(Packet_list);
	Packet_list_init_beg(conn->sent_packets);
	conn->bytes_sent = 0;
	conn->bytes_recv = 0;

	if (is_pkt_outgoing(pkt))
	{
		conn->bytes_sent += pkt->len;
		addPacket(conn->sent_packets, pkt);
		conn->ref_packet = g_slice_new(Packet);
		Packet_init(conn->ref_packet, *pkt);
	}
	else
	{
		conn->bytes_recv += pkt->len;
		addPacket(conn->received_packets, pkt);
		conn->ref_packet = get_inverted_packet(pkt);
	}
	conn->last_packet_time = pkt->time.tv_sec;
}

Connection *
find_connection_with_matching_source(Packet *pkt)
{
	Conn_list *current = connections;
	while (current != NULL)
	{
		if (packet_match_source(pkt, Conn_list_get_connection(current)->ref_packet))
			return current->conn;
		current = current->next;
	}
	return NULL;
}


Connection *
find_connection_with_matching_ref_packet_or_source(Packet *pkt)
{
	Conn_list *current = connections;
	while (current != NULL)
	{
		if (packet_match(pkt, Conn_list_get_connection(current)->ref_packet))
			return current->conn;
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

int 
Connection_get_last_packet_time(Connection *conn)
{
	return conn->last_packet_time;
}

u_int64_t Packet_list_sum_and_del(Packet_list *pktlist, timeval t)
{
	u_int64_t sum = 0;
	Packet_list_node *current = pktlist->content;
	Packet_list_node *previous ;
	while (current != NULL)
	{
		if (current->pkt->time.tv_sec <= t.tv_sec - PERIOD)
		{
			if (current == pktlist->content)
				pktlist->content = NULL;
			else if (previous != NULL)
				previous->next = NULL;
			g_slice_free(Packet_list_node, current);
			return sum;
		}
		sum += current->pkt->len;
		previous = current;
		current = current->next;
	}
	return sum;
}
void
Connection_sum_and_del(Connection *conn, timeval t, u_int64_t *recv, u_int64_t *sent)
{
	*sent = 0;
	*recv = 0;
	*sent = Packet_list_sum_and_del(conn->sent_packets, t);
	*recv = Packet_list_sum_and_del(conn->received_packets, t);
}