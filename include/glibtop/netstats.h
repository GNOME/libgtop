#ifndef __GLIBTOP_NETSTATS_H_
#define __GLIBTOP_NETSTATS_H_

#include <glib.h>

#include <glibtop.h>
#include <glibtop/global.h>


G_BEGIN_DECLS

#define GLIBTOP_NETSTATS_NUMBER	0
#define GLIBTOP_MAX_NETSTATS 	1

#if GLIBTOP_SUID_NETSTATS
GArray *network_stats_get_global_instance_s(glibtop *server, GArray *val);
void _glibtop_init_netstats_s(glibtop *server);
void glibtop_init_packet_capture_s (void);
#endif

G_END_DECLS

#endif
