#include <glibtop.h>

#include <glibtop/fsusage.h>
#include <glibtop/mountlist.h>

#include <stdio.h>


static void print_fsusage(const char *mountpoint)
{
  glibtop_fsusage buf;

  glibtop_get_fsusage(&buf, mountpoint);

  printf("%-20s %-10llu %-10llu %-10llu %.1f\n",
	 mountpoint,
	 buf.blocks * buf.block_size >> 20,
	 (buf.blocks - buf.bavail) * buf.block_size >> 20,
	 buf.bavail * buf.block_size >> 20,
	 (buf.blocks - buf.bavail) * 100.0 / buf.blocks
	 );
}


int main()
{
  glibtop_mountlist buf;
  glibtop_mountentry *entries;
  size_t i;

  glibtop_init();

  printf("%-20s %-10s %-10s %-10s %-10s\n",
	 "Filesystem", "Size", "Used", "Avail", "Use%");

  entries = glibtop_get_mountlist(&buf, FALSE);

  for(i = 0; i < buf.number; ++i)
    {
      print_fsusage(entries[i].mountdir);
    }

  g_free(entries);

  glibtop_close();

  return 0;
}
