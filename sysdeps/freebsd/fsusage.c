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
#if 0
#include <libgeom.h>
#include <sys/resource.h>
#include <devstat.h>
#include <sys/devicestat.h>
#endif

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
#if 0
        struct devstat *ds;
        void *sc;
        struct timespec ts;
        struct gprovider *gp;
        struct gident *gid;
        struct gmesh gmp;
        double etime;
        uint64_t ld[2];
#endif

        result = statfs (path, &sfs);

        if (result == -1) {
                glibtop_warn_io_r (server, "statfs");
                return;
        }
#if 0
        ld[0] = 0;
        ld[1] = 0;
        result = geom_gettree (&gmp);
        if (result != 0) {
                glibtop_warn_io_r (server, "geom_gettree = %d", result);
                return;
        }

        result = geom_stats_open ();
        if (result != 0) {
                glibtop_warn_io_r (server, "geom_stats_open()");
                geom_deletetree (&gmp);
                return;
        }

        sc = NULL;
        sc = geom_stats_snapshot_get ();
        if (sc == NULL) {
                glibtop_warn_io_r (server, "geom_stats_snapshot_get()");
                geom_stats_close ();
                geom_deletetree (&gmp);
                return;
        }
        geom_stats_snapshot_timestamp (sc, &ts);
        etime = ts.tv_sec + (ts.tv_nsec * 1e-9);
        geom_stats_snapshot_reset (sc);

        for (;;) {
                ds = geom_stats_snapshot_next (sc);
                if (ds == NULL) {
                        break;
                }
                if (ds->id == NULL) {
                        continue;
                }

                gid = geom_lookupid (&gmp, ds->id);
                if (gid == NULL) {
                        geom_deletetree (&gmp);
                        result = geom_gettree (&gmp);
                        gid = geom_lookupid (&gmp, ds->id);
                }

                if (gid == NULL) {
                        continue;
                }
                if (gid->lg_what == ISCONSUMER) {
                        continue;
                }

                gp = gid->lg_ptr;

                if (!g_str_has_suffix (sfs.f_mntfromname, gp->lg_name)) {
                        continue;
                } else {
                        result = devstat_compute_statistics (ds, NULL, etime,
                                                             DSM_TOTAL_TRANSFERS_READ,
                                                             &ld[0],
                                                             DSM_TOTAL_TRANSFERS_WRITE,
                                                             &ld[1], DSM_NONE);
                        if (result != 0) {
                                glibtop_warn_io_r (server,
                                                   "devstat_compute_statistics()");
                                geom_stats_snapshot_free (sc);
                                geom_stats_close ();
                                geom_deletetree (&gmp);
                                return;
                        }
                        break;
                }
        }

        geom_stats_snapshot_free (sc);
        geom_stats_close ();
        geom_deletetree (&gmp);

        buf->read = ld[0];
        buf->write = ld[1];
#else
        buf->read = sfs.f_syncreads + sfs.f_asyncreads;
        buf->write = sfs.f_syncwrites + sfs.f_asyncwrites;
#endif
	if (buf->read || buf->write) {
                buf->flags |= (1 << GLIBTOP_FSUSAGE_READ) | (1 << GLIBTOP_FSUSAGE_WRITE);
	}
}

void
glibtop_get_fsusage_s(glibtop *server, glibtop_fsusage *buf, const char *path)
{
	struct statvfs fsd;

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

