#include <config.h>
#include <glibtop.h>
#include <glibtop/fsusage.h>
#include <glibtop/error.h>

#include "glibtop_private.h"

#include <glib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/kdev_t.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * Linux 2.6.x
 * linux/Documentation/iostats.txt
 */




static char *
get_partition(const char *mountpoint)
{
	FILE *partitions;
	char *name = NULL;
	char line[1024];
	struct stat statb;

	if(stat(mountpoint, &statb) == -1)
		return NULL;

	if((partitions = fopen("/proc/partitions", "r")) == NULL)
		return NULL;

	while(fgets(line, sizeof line, partitions))
	{
		unsigned major, minor;
		char dev[32];

		if(sscanf(line, "%u %u %*u %31s", &major, &minor, dev) != 3)
			continue;

		if(MKDEV(major, minor) != statb.st_dev)
			continue;

		name = g_strdup(dev);
		break;
	}

	fclose(partitions);
	return name;
}


/*
  Bug #539360.
  /sys/.../stat format is partially defined in
  linux/Documentation/block/stat.txt (looks outdated).  Before linux
  2.5.25, /sys/block/%s/stat and /sys/block/%s/%s/stat were not the
  same, but the following commit changed the latter to have the same
  format and broke compatibility.

  Commit 34e8beac92c27d292938065f8375842d2840767c
  Author: Jerome Marchand <jmarchan@redhat.com>
  Date:   Fri Feb 8 11:04:55 2008 +0100

    Enhanced partition statistics: sysfs

    Reports enhanced partition statistics in sysfs.

    Signed-off-by: Jerome Marchand <jmarchan@redhat.com>

    fs/partitions/check.c |   22 +++++++++++++++++++---
    1 files changed, 19 insertions(+), 3 deletions(-)

 */

static void
get_sys_path(glibtop* server, const char *device, char **stat_path, const char **parse_format)
{
	const char* linux_2_6_25_format = "%*llu %*llu %llu %*llu"
					  "%*llu %*llu %llu %*llu";

	if(g_str_has_prefix(device, "hd") || g_str_has_prefix(device, "sd"))
	{
		char *prefix;
		char *path;
		size_t offset;

		offset = strcspn(device, "0123456789");

		prefix = g_strdup(device);
		prefix [offset] = '\0';

		path = g_strdup_printf("/sys/block/%s/%s/stat",
				       prefix, device);

		g_free(prefix);

		*stat_path = path;
		if (server->os_version_code < LINUX_VERSION_CODE(2, 6, 25))
			*parse_format = "%*llu %llu %*llu %llu";
		else
			*parse_format = linux_2_6_25_format;
	}
	else
	{
		*stat_path = g_strdup_printf("/sys/block/%s/stat", device);
		if (server->os_version_code < LINUX_VERSION_CODE(2, 6, 25))
			*parse_format = "%*llu %*llu %llu %*llu %*llu %*llu %llu";
		else
			*parse_format = linux_2_6_25_format;
	}
}



static void linux_2_6_0(glibtop *server, glibtop_fsusage *buf, const char *path)
{
	char *device;
	char *filename;
	const char *format;
	int ret;
	char buffer[BUFSIZ];

	device = get_partition(path);
	if(!device) return;

	get_sys_path(server, device, &filename, &format);
	g_free(device);

	ret = try_file_to_buffer(buffer, sizeof buffer, filename);

	if(ret < 0) return;

	if (sscanf(buffer, format, &buf->read, &buf->write) != 2) {
		glibtop_warn_io_r(server, "Could not parse %s", filename);
		return;
	}

	g_free(filename);

	buf->flags |= (1 << GLIBTOP_FSUSAGE_READ) | (1 << GLIBTOP_FSUSAGE_WRITE);
}


static void linux_2_4_0(glibtop *server, glibtop_fsusage *buf, const char *path)
{
}


static void
get_fsusage_read_write(glibtop *server, glibtop_fsusage *buf, const char *path)
{
  if(server->os_version_code >= LINUX_VERSION_CODE(2, 6, 0))
    {
      linux_2_6_0(server, buf, path);
    }
  else if(server->os_version_code >= LINUX_VERSION_CODE(2, 4, 0))
    {
      linux_2_4_0(server, buf, path);
    }
}


/* the following comes from sysdeps/common/mountlist.c if copyright matters...
 */

static const unsigned long _glibtop_sysdeps_fsusage =
(1L << GLIBTOP_FSUSAGE_BLOCKS) + (1L << GLIBTOP_FSUSAGE_BFREE)
+ (1L << GLIBTOP_FSUSAGE_BAVAIL) + (1L << GLIBTOP_FSUSAGE_FILES)
+ (1L << GLIBTOP_FSUSAGE_FFREE) + (1L << GLIBTOP_FSUSAGE_BLOCK_SIZE);



void
glibtop_get_fsusage_s(glibtop *server, glibtop_fsusage *buf, const char *path)
{
  struct statvfs fsd;

  glibtop_init_r(&server, 0, 0);
  memset(buf, 0, sizeof(glibtop_fsusage));

  if (statvfs(path, &fsd) < 0) {
    glibtop_warn_r(server, "statvfs '%s' failed", path);
    return;
  }

  buf->blocks = fsd.f_blocks;
  buf->bfree  = fsd.f_bfree;
  buf->bavail = (fsd.f_bavail > fsd.f_bfree) ? 0 : fsd.f_bavail;
  buf->files  = fsd.f_files;
  buf->ffree  = fsd.f_ffree;
  buf->block_size = fsd.f_bsize;
  buf->flags = _glibtop_sysdeps_fsusage;

  /* setting additional flags is delegated */
  get_fsusage_read_write(server, buf, path);
}
