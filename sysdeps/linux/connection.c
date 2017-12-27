#include "connection.c"
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
	Packet_list_node *pktlistnode = (Packet_list_node *)malloc(sizeof(Packet_list_node));
	Packet_list_node_init(pktlistnode, pkt_val,NULL);
	pktlist->content = pktlistnode;
}

//adding a packet to the linked list
void addPacket(Packet_list *pktlist, Packet *pkt)
{
	if(pklist->content == NULL)
	{	
		Packet *copy_pkt = (Packet*)malloc(sizeof(Packet));
		Packet_init(copy_pkt,*pkt);
		Packet_list_node *pktNode = (Packet_list_node *)malloc(pktNode);
		Packet_list_node_init(pktNode, copy_pkt, NULL);
		pktlist->content = pktNode;
		return;
	}
	if(pklist->content->pkt->time.tv_sec == pkt->time.tv_sec)
	{
		pklist->content->pkt->len += pkt->len;
		return;
	}

	//add a new node to the list and make this new node as the content of the Packet_list
	//currently a copy of the packet is made so that later we can delete the orginal capture
	Packet *copy_pkt = (Packet*)malloc(sizeof(Packet));
	Packet_init(copy_pkt,*pkt);
	Packet_list_node *pktNode = (Packet_list_node *)malloc(pktNode);
	Packet_list_node_init(pktNode, copy_pkt, pktlist->content);
	pktlist->content = pktNode;
}

//this packet to get the time of the last pcket capture for th given connection
void add_packet_to_connection(Connection *conn, Packet *pkt)
{
	connection->last_packet_time = pkt->time.tv_sec;
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
Conn_list_init(Conn_list *clist,Connection *conn_val, Conn_list *next_val = NULL)
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
setNext(Conn_list *clist,Conn_list *next_val)
{
	clist->next = next_val;
}

ConnList *
Connection_list_get_next(Conn_list *clist)
{
	return clist->next;
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
