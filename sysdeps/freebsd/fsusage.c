#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/fsusage.h>

#include <glibtop_suid.h>

#include <glib.h>

#include <unistd.h>
#include <sys/param.h>
#if defined (HAVE_SYS_STATVFS_H)
#include <sys/statvfs.h>
#else
#include <sys/mount.h>
#endif

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
#if defined (STAT_STATVFS)
	struct statvfs sfs;
#else
	struct statfs sfs;
#endif

#if defined (STAT_STATVFS)
	result = statvfs (path, &sfs);
#else
	result = statfs (path, &sfs);
#endif

	if (result == -1) {
		return;
	}
#if !defined(__FreeBSD_kernel__)
	buf->read = sfs.f_syncreads + sfs.f_asyncreads;
	buf->write = sfs.f_syncwrites + sfs.f_asyncwrites;
#endif
	buf->flags |= (1 << GLIBTOP_FSUSAGE_READ) | (1 << GLIBTOP_FSUSAGE_WRITE);
}
