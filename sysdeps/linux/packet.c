#include <glibtop/packet.h>
#include <glib.h>
#include <glibtop/interface_local_addr.h>
#include <glibtop/dev_handles.h>
#include <arpa/inet.h>
#include <stdlib.h>

void
Packet_init_in_addr(Packet *pkt, struct in_addr pkt_sip, unsigned short pkt_sport, struct in_addr pkt_dip, 
						unsigned short pkt_dport, guint32 pkt_len, struct timeval pkt_ts ,direction pkt_dir)
{
	pkt->sa_family = AF_INET;
	pkt->sip = pkt_sip;
	pkt->dip = pkt_dip;
	pkt->sport = pkt_sport;
	pkt->dport = pkt_dport;
	pkt->len = pkt_len;
	pkt->time = pkt_ts;
	if(pkt_dir)
		pkt->dir = pkt_dir;
	else
		pkt->dir = dir_unknown;
	pkt->pkt_hash = NULL;
}

void
Packet_init_in6_addr(Packet *pkt, struct in6_addr pkt_sip6, unsigned short pkt_sport, struct in6_addr pkt_dip6, 
						unsigned short pkt_dport, guint32 pkt_len, struct timeval pkt_ts ,direction pkt_dir) 
{
	pkt->sa_family = AF_INET6;
	pkt->sip6 = pkt_sip6;
	pkt->dip6 = pkt_dip6;
	pkt->sport = pkt_sport;
	pkt->dport = pkt_dport;
	pkt->len = pkt_len;
	pkt->time = pkt_ts;
	if(pkt_dir)
		pkt->dir = pkt_dir;
	else
		pkt->dir = dir_unknown;
	pkt->pkt_hash = NULL;
}

void
Packet_init(Packet *pkt, Packet *old_packet)
{
	pkt->sa_family = old_packet->sa_family;
	pkt->sip = old_packet->sip;
	pkt->dip = old_packet->dip;
	pkt->sip6 = old_packet->sip6;
	pkt->dip6 = old_packet->dip6;
	pkt->sport = old_packet->sport;
	pkt->dport = old_packet->dport;
	pkt->len = old_packet->len;
	pkt->time = old_packet->time;
	pkt->dir = old_packet->dir;
	pkt->pkt_hash = old_packet->pkt_hash;
}

//check the dir of the packet
gboolean 
is_pkt_outgoing(Packet *pkt)
{
	local_addr *pkt_interface_local_addr = get_local_addr_instance(NULL);
	g_assert(pkt_interface_local_addr !=  NULL);
	gboolean is_local;
	switch(pkt->dir)
	{
	case dir_outgoing:
		return TRUE;
	case dir_incoming:
		return FALSE;
	case dir_unknown:

		if (pkt->sa_family == AF_INET)
			is_local = local_addr_contains(pkt_interface_local_addr, &pkt->sip.s_addr);
		else
			is_local = local_addr6_contains(pkt_interface_local_addr, &pkt->sip6);
		
		if (is_local)
		{
			pkt->dir = dir_outgoing;
			return TRUE;
		}
		else
		{
			if (pkt->sa_family == AF_INET)
				is_local = local_addr_contains(pkt_interface_local_addr, &pkt->dip.s_addr);
			else
				is_local = local_addr6_contains(pkt_interface_local_addr, &pkt->dip6);

			if (!is_local)
			{
				printf("neither dip nor sip are local\n");
				return FALSE;
			}
			pkt->dir = dir_incoming;
			return FALSE;
		}
	}
	return FALSE;
}

gboolean
packet_match_source(Packet *pkt, Packet *other)
{
	return (pkt->sport == other->sport) &&
			(pkt->sa_family == AF_INET) ? (pkt->sip.s_addr == other->sip.s_addr) :
										(pkt->sip6.s6_addr == other->sip6.s6_addr);

}

gboolean
packet_match(Packet *pkt, Packet *other) 
{
	return (pkt->sa_family == other->sa_family) && 
			(pkt->sport == other->sport) &&
			(pkt->dport == other->dport) &&
			(pkt->sa_family == AF_INET) ? ((pkt->sip.s_addr == other->sip.s_addr) && (pkt->dip.s_addr == other->dip.s_addr)):
										((pkt->sip6.s6_addr == other->sip6.s6_addr) && (pkt->dip6.s6_addr == other->dip6.s6_addr));
}

static direction
invert(direction dir)
{
	if (dir == dir_incoming)
		return dir_outgoing;
	else if (dir_outgoing)
		return dir_incoming;
	else
		return dir_unknown;
}

Packet *
get_inverted_packet(Packet *pkt)
{
	direction inverted_dir = invert(pkt->dir);
	Packet *inverted_packet = g_slice_new (Packet);
	if (pkt->sa_family == AF_INET)
		Packet_init_in_addr(inverted_packet, pkt->dip, pkt->dport, pkt->sip, pkt->sport, 
							pkt->len, pkt->time, inverted_dir);
	else
		Packet_init_in6_addr(inverted_packet, pkt->dip6, pkt->dport, pkt->sip6, pkt->sport,
							pkt->len, pkt->time, inverted_dir);
	return inverted_packet; 
}

char *
Packet_gethash(Packet *pkt)
{
	if (pkt->pkt_hash != NULL)
	{
		return pkt->pkt_hash;
	}	
	//free pkt_hash
	char pkt_hash[92];
	char local_string[128];
	char remote_string[128];
	if (pkt->sa_family == AF_INET)
	{
		inet_ntop(pkt->sa_family, &(pkt->sip), local_string, 16);
		inet_ntop(pkt->sa_family, &(pkt->dip), remote_string, 16);
	}
	else 
	{
		inet_ntop(pkt->sa_family, &(pkt->sip6), local_string, 46);
		inet_ntop(pkt->sa_family, &(pkt->dip6), remote_string, 46);
	}
	if (is_pkt_outgoing(pkt))
		{snprintf(pkt_hash, HASHKEYSIZE*sizeof(char), "%s:%d-%s:%d",
				local_string, pkt->sport, remote_string, pkt->dport);
}
	else
	{
		snprintf(pkt_hash, HASHKEYSIZE*sizeof(char), "%s:%d-%s:%d",
				remote_string, pkt->dport, local_string, pkt->sport);
	}
	pkt->pkt_hash = g_strdup(pkt_hash);
	printf("matching inode hash %s:%d-%s:%d\n", local_string, pkt->sport, remote_string, pkt->dport);
	if (pkt->pkt_hash != NULL)
		return pkt->pkt_hash;
	return NULL;
}
