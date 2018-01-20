#include <glibtop/connection.h>
#include <glibtop/packet.h>
#include <stdio.h>

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
	Packet_list_node_init(pktlistnode, pkt_val, NULL);
	pktlist->content = pktlistnode;
}

//adding a packet to the linked list
void addPacket(Packet_list *pktlist, Packet *pkt)
{
	if (pktlist->content == NULL)
	{	
		Packet *copy_pkt = g_slice_new(Packet);
		Packet_init(copy_pkt, pkt);
		Packet_list_node *pktNode = g_slice_new(Packet_list_node);
		Packet_list_node_init(pktNode, copy_pkt, NULL);
		pktlist->content = pktNode;
		return;
	}
	if (pktlist->content->pkt != NULL)
	{
		if (pktlist->content->pkt->time.tv_sec == pkt->time.tv_sec)
		{
			pktlist->content->pkt->len += pkt->len;
			return;
		}
	}
	//add a new node to the list and make this new node as the content of the Packet_list
	//currently a copy of the packet is made so that later we can delete the orginal capture
	//printf("Packet added to existing packetlist");
	Packet *copy_pkt = g_slice_new(Packet);
	Packet_init(copy_pkt, pkt);
	Packet_list_node *pktNode = g_slice_new(Packet_list_node);
	Packet_list_node_init(pktNode, copy_pkt, pktlist->content);
	pktlist->content = pktNode;
}

//this packet to get the time of the last pcket capture for th given connection
void 
add_packet_to_connection(Connection *conn, Packet *pkt)
{	printf("ADDING to %d:%d pkt %d:%d\n", conn->ref_packet->sport, conn->ref_packet->dport, pkt->sport, pkt->dport);
	conn->last_packet_time = pkt->time.tv_sec;
	//if outgoing or incoming accordingly add the packet to respective packet list sent or recv i.e malloc a new 
	if (is_pkt_outgoing(pkt)) //check packet.c
	{
		conn->bytes_sent += pkt->len;
		addPacket(conn->sent_packets, pkt);
	}
	else
	{
		conn->bytes_recv += pkt->len;
		addPacket(conn->received_packets, pkt);
	}
}

void
Conn_list_init(Conn_list *clist, Connection *conn_val, Conn_list *next_val)
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

Conn_list *
get_global_connections_instance(Conn_list *val)
{
	static Conn_list *global_connections_list = NULL;
	if (val != NULL)
		global_connections_list = val;
	else if (global_connections_list == NULL)
		{
			global_connections_list = g_slice_new(Conn_list);
			Conn_list_init(global_connections_list, NULL, NULL);
		}
	return global_connections_list;
}

void
Connection_init(Connection *conn, Packet *pkt)
{
	Conn_list *temp = g_slice_new(Conn_list);
	Conn_list *connections = get_global_connections_instance(NULL);
	Conn_list_init(temp, conn, connections);
	get_global_connections_instance(temp); //to set connections = temp in the static var global_connections_list
	conn->sent_packets = g_slice_new(Packet_list);
	Packet_list_init_beg(conn->sent_packets);
	conn->received_packets = g_slice_new(Packet_list);
	Packet_list_init_beg(conn->sent_packets);
	conn->bytes_sent = 0;
	conn->bytes_recv = 0;
	printf("new conn w/ pkt len = %d\n", pkt->len );
	if (is_pkt_outgoing(pkt))
	{
		conn->bytes_sent += pkt->len;
		addPacket(conn->sent_packets, pkt);
		conn->ref_packet = g_slice_new(Packet);
		Packet_init(conn->ref_packet, pkt);
		printf("New reference packet created at %d: \n",conn->ref_packet->sport );
	}
	else
	{
		conn->bytes_recv += pkt->len;
		addPacket(conn->received_packets, pkt);
		conn->ref_packet = get_inverted_packet(pkt);
		printf("New reference packet created at %d:\n", conn->ref_packet->sport );
	}
	conn->last_packet_time = pkt->time.tv_sec;
	
}

Connection *
find_connection_with_matching_source(Packet *pkt)
{	
	Conn_list *current = get_global_connections_instance(NULL);
	while (current->conn != NULL)
	{
		if (packet_match_source(pkt, Conn_list_get_connection(current)->ref_packet))
			return current->conn;
		current = current->next;
	}
	return NULL;
}

//can be used later to debug 
void 
print_packet_list(Connection *conn)
{	
	int i = 1;
	Packet_list_node *sent_packets;
	Packet_list_node *received_packets;
	if (conn->sent_packets)
		sent_packets = conn->sent_packets->content;
	if (conn->received_packets)
		received_packets = conn->received_packets->content;
	printf("SENT PACKETS\n");
	Packet_list_node *previous = NULL;
	while (sent_packets != NULL && previous != sent_packets)
	{	
		if (sent_packets->pkt)
		{
			printf("%d. %dbytes\n", i++, sent_packets->pkt->len);
			previous = sent_packets;
			sent_packets = sent_packets->next;
		}
		else
			break;
	}
	
	previous = NULL;
	printf("received_packets\n");
	i = 1;
	while (received_packets != NULL && previous != received_packets)
	{	if (received_packets->pkt)
		{
			printf("%d. %dbytes\n", i++, received_packets->pkt->len);
			previous = received_packets;
			received_packets = received_packets->next;
		}
		else
			return;
	}
}

//can be used to debug
void 
print_global_connection_list()
{	
	printf("CONNECTION LIST\n");
	Conn_list *current = get_global_connections_instance(NULL);
	while (current->conn != NULL)
	{
		printf("bytes_recv:%d from%d bytes sent %d sent %d\n",current->conn->bytes_recv, 
															current->conn->ref_packet->sport, 
															current->conn->bytes_sent, 
															current->conn->ref_packet->dport);
		print_packet_list(current->conn);
		current = current->next;
	}
}

Connection *
find_connection_with_matching_ref_packet_or_source(Packet *pkt)
{
	Conn_list *current = get_global_connections_instance(NULL);
	while (current->conn != NULL)
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

u_int64_t Packet_list_sum_and_del(Packet_list *pktlist, struct timeval t)
{
	u_int64_t sum = 0;
	int i=0;
	Packet_list_node *current = pktlist->content;
	Packet_list_node *previous = NULL;
	while (current != NULL && previous != current && current->pkt)
	{	
		if (!(current->pkt->time.tv_sec <= t.tv_sec - PERIOD))
		{ 
			sum += current->pkt->len;
		}
		previous = current;
		current = current->next;
	}
	return sum;
}

void
Connection_sum_and_del(Connection *conn, struct timeval t, u_int64_t *recv, u_int64_t *sent)
{
 	*sent = 0;
	*recv = 0;
	if(conn->sent_packets->content == NULL)
		printf("null packet list\n");
	*sent = Packet_list_sum_and_del(conn->sent_packets, t);
	*recv = Packet_list_sum_and_del(conn->received_packets, t);
}
