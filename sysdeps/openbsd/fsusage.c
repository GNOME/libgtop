#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/fsusage.h>

#include <glibtop_suid.h>

#include <glib.h>

#include <unistd.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/statvfs.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const unsigned long _glibtop_sysdeps_fsusage =
(1L << GLIBTOP_FSUSAGE_BLOCKS) + (1L << GLIBTOP_FSUSAGE_BFREE)
+ (1L << GLIBTOP_FSUSAGE_BAVAIL) + (1L << GLIBTOP_FSUSAGE_FILES)
+ (1L << GLIBTOP_FSUSAGE_FFREE) + (1L << GLIBTOP_FSUSAGE_BLOCK_SIZE);

static void
_glibtop_get_fsusage_read_write (glibtop *server, glibtop_fsusage *buf, const char *path)
{
        int result;
        struct statfs sfs;

        result = statfs (path, &sfs);

        if (result == -1) {
                glibtop_warn_io_r (server, "statfs");
                return;
        }

        buf->read = sfs.f_syncreads + sfs.f_asyncreads;
        buf->write = sfs.f_syncwrites + sfs.f_asyncwrites;
        buf->flags |= (1 << GLIBTOP_FSUSAGE_READ) | (1 << GLIBTOP_FSUSAGE_WRITE);
}

void
glibtop_get_fsusage_s(glibtop *server, glibtop_fsusage *buf, const char *path)
{
	struct statvfs fsd;

	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_fsusage));

	if (statvfs (path, &fsd) < 0)
		return;

	buf->block_size = fsd.f_frsize;
	buf->blocks = fsd.f_blocks;
	buf->bfree  = fsd.f_bfree;
	buf->bavail = (fsd.f_bavail > fsd.f_bfree) ? 0 : fsd.f_bavail;
	buf->files  = fsd.f_files;
	buf->ffree  = fsd.f_ffree;

	buf->flags = _glibtop_sysdeps_fsusage;

	_glibtop_get_fsusage_read_write(server, buf, path);
}

