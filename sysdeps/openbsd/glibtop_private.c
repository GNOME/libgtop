#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>

#include "glibtop_private.h"

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>
#include <errno.h>

/* Ported from linux/glibtop_private.c */
gboolean
safe_readlink(const char *path, char *buf, int bufsiz)
{
	int ret;

	ret = readlink(path, buf, bufsiz - 1);

	if (ret == -1) {
		g_warning("Could not read link %s : %s", path, strerror(errno));
		return FALSE;
	}

	buf[ret] = '\0';
	return TRUE;
}
