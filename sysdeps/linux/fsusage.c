#include <glibtop.h>
#include <glibtop/fsusage.h>

#include "glibtop_private.h"

#include <glib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/kdev_t.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
G_GNUC_INTERNAL
_glibtop_linux_get_fsusage_read_write(glibtop *server,
				      glibtop_fsusage *buf,
				      const char *path);

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

	g_return_val_if_fail((partitions = fopen("/proc/partitions", "r")), NULL);

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


static char *
get_sys_path(const char *device)
{
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
		return path;

	}
	else
	{
		return g_strdup_printf("/sys/block/%s/stat", device);
	}
}



static void linux_2_6_0(glibtop *server, glibtop_fsusage *buf, const char *path)
{
	char *device;
	char *filename;
	int ret;
	char buffer[BUFSIZ];
	char *p;

	device = get_partition(path);
	if(!device) return;

	filename = get_sys_path(device);
	g_free(device);

	ret = try_file_to_buffer(buffer, filename);
	g_free(filename);

	if(ret < 0) return;

	p = buffer;
	p = skip_token(p);
	buf->read = strtoull(p, &p, 0);
	p = skip_token(p);
	buf->write = strtoull(p, &p, 0);

	buf->flags |= (1 << GLIBTOP_FSUSAGE_READ) | (1 << GLIBTOP_FSUSAGE_WRITE);
}


static void linux_2_4_0(glibtop *server, glibtop_fsusage *buf, const char *path)
{
}


void G_GNUC_INTERNAL
_glibtop_linux_get_fsusage_read_write(glibtop *server,
				      glibtop_fsusage *buf,
				      const char *path)
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
