#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/fsusage.h>

#include <glibtop_suid.h>

#include <glib.h>

#include <unistd.h>
#include <sys/param.h>
#include <sys/mount.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
G_GNUC_INTERNAL
_glibtop_freebsd_get_fsusage_read_write(glibtop *server,
				      glibtop_fsusage *buf,
				      const char *path);

void G_GNUC_INTERNAL
_glibtop_freebsd_get_fsusage_read_write(glibtop *server,
				      glibtop_fsusage *buf,
				      const char *path)
{
	int result;
	struct statfs sfs;

	result = statfs (path, &sfs);

	if (result == -1) {
		return;
	}

	buf->read = sfs.f_syncreads + sfs.f_asyncreads;
	buf->write = sfs.f_syncwrites + sfs.f_asyncwrites;

	buf->flags |= (1 << GLIBTOP_FSUSAGE_READ) | (1 << GLIBTOP_FSUSAGE_WRITE);
}
