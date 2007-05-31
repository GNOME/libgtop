#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/fsusage.h>

#include <glibtop_suid.h>

#include <glib.h>

#include <unistd.h>
#include <sys/param.h>
#include <sys/mount.h>
#if __FreeBSD_version >= 600000 || defined(__FreeBSD_kernel__)
#include <libgeom.h>
#include <sys/resource.h>
#include <devstat.h>
#include <sys/devicestat.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
_glibtop_freebsd_get_fsusage_read_write(glibtop *server,
                                        glibtop_fsusage *buf,
                                        const char *path);

void
_glibtop_freebsd_get_fsusage_read_write(glibtop *server,
                                        glibtop_fsusage *buf,
                                        const char *path)
{
        int result;
        struct statfs sfs;
#if __FreeBSD_version >= 600000 || defined(__FreeBSD_kernel__)
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
#if __FreeBSD_version >= 600000 || defined(__FreeBSD_kernel__)
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
        buf->flags |= (1 << GLIBTOP_FSUSAGE_READ) | (1 << GLIBTOP_FSUSAGE_WRITE);
}
