#include <glibtop.h>
#include <glibtop/fsusage.h>

#include <glib.h>

#include <mntent.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void _glibtop_linux_get_fsusage_read_write(glibtop *server,
					   glibtop_fsusage *buf,
					   const char *path);

/*
 * Linux 2.6.x
 * linux/Documentation/iostats.txt
 */

static void linux_2_6_0(glibtop *server, glibtop_fsusage *buf, const char *path)
{
  FILE *mtab = setmntent("/etc/mtab", "r");

  struct mntent *emnt;

  while((emnt = getmntent(mtab)) != NULL)
    {
      if(strcmp(emnt->mnt_dir, path) != 0)
	continue;

      char filename[64]; /* magic */

      char buffer[1024]; /* magic */
      char *p;

      char device[32]; /* magic */
      unsigned partition;

      const char *devname = emnt->mnt_fsname;
      size_t offset;

      /*
	we only deal with /dev block devices "/dev/<DEVICE><PARTITION>"
	i don't know if other block devices such as network block
	devices provide this kind of information.
      */

      if(!g_str_has_prefix(devname, "/dev/"))
	break;

      /* skip the "/dev/" */
      devname += sizeof "/dev/" - 1;

      g_strlcpy(device, devname, sizeof device);

      offset = strcspn(devname, "0123456789");
      partition = strtoul(devname + offset, NULL, 0);

      device[offset] = '\0';

      if((size_t) g_snprintf(filename, sizeof filename,
			     "/sys/block/%s/%s%u/stat",
			     device, device, partition) >= sizeof filename)
	break;


      if(try_file_to_buffer(buffer, filename) < 0)
	break;

      p = buffer;
      p = skip_token(p);
      buf->read = strtoull(p, &p, 0);
      p = skip_token(p);
      buf->write = strtoull(p, &p, 0);
    }

  endmntent(mtab);
}


static void linux_2_4_0(glibtop *server, glibtop_fsusage *buf, const char *path)
{
}


void _glibtop_linux_get_fsusage_read_write(glibtop *server,
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
