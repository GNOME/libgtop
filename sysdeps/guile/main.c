#include "guile.h"
#include "structures.h"
#include <libguile/snarf.h>

long scm_glibtop_smob_tags [GLIBTOP_MAX_STRUCTURES];
SCM scm_glibtop_global_server_smob;

SCM_GLOBAL_VCELL (s_glibtop_global_server, "glibtop-global-server");

void
glibtop_boot_guile_main (void)
{
#ifndef SCM_MAGIC_SNARFER
#include "main.x"
#endif

    SCM_NEWSMOB (scm_glibtop_global_server_smob,
		 scm_glibtop_smob_tags [GLIBTOP_STRUCTURE_GLIBTOP],
		 glibtop_global_server);

    glibtop_server_ref (glibtop_global_server);

    SCM_SETCDR (s_glibtop_global_server, scm_glibtop_global_server_smob);
}
