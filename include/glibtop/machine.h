#ifndef GLIBTOP_SYSDEPS_COMMON_MACHINE_H_1437506041
#define GLIBTOP_SYSDEPS_COMMON_MACHINE_H_1437506041

#include <glib.h>
#include <glibtop_machine.h>

/* private */
static inline void
glibtop_machine_new(glibtop *server)
{
	server->machine = g_new0(glibtop_machine, 1);
}

/* private */
static inline void
glibtop_machine_delete(glibtop *server)
{
	g_free(server->machine);
	server->machine = NULL;
}

#endif /* GLIBTOP_SYSDEPS_COMMON_MACHINE_H_1437506041 */
