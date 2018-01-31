#ifndef __GLIBTOP_PACKET_H_
#define __GLIBTOP_PACKET_H_

#include <glib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>

G_BEGIN_DECLS

#define HASHKEYSIZE 92
typedef enum _direction direction;
enum _direction
{
	dir_unknown,
	dir_incoming,
	dir_outgoing
};

typedef struct _Packet Packet;
struct _Packet
{
	direction dir;
	int sa_family;
	struct in_addr sip;
	struct in_addr dip;
	struct in6_addr sip6;
	struct in6_addr dip6;
	unsigned short sport;
	unsigned short dport;
	guint32 len;
	struct timeval time;
	char *pkt_hash;
};

void Packet_init_in_addr(Packet *pkt, struct in_addr pkt_sip, unsigned short pkt_sport, struct in_addr pkt_dip, 
						unsigned short pkt_dport, guint32 pkt_len, struct timeval pkt_ts, direction pkt_dir);
void Packet_init_in6_addr(Packet *pkt, struct in6_addr pkt_sip6, unsigned short pkt_sport, struct in6_addr pkt_dip6, 
						unsigned short pkt_dport, guint32 pkt_len, struct timeval pkt_ts, direction pkt_dir);
void Packet_init(Packet *pkt, Packet *old_packet);
gboolean is_pkt_outgoing(Packet *pkt);
gboolean packet_match_source(Packet *pkt, Packet *other);
gboolean packet_match(Packet *pkt,Packet *other); 
Packet *get_inverted_packet(Packet *pkt);
char *Packet_gethash(Packet *pkt);

G_END_DECLS

#endif
