#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <glibtop.h>

#include <glibtop/fsusage.h>
#include <glibtop/mountlist.h>

#include <stdio.h>


static void print_fsusage(const char *mountpoint)
{
  glibtop_fsusage buf;

  glibtop_get_fsusage(&buf, mountpoint);

  printf("%-30s %10llu %10llu %10llu %5.1f %10llu %10llu\n",
	 mountpoint,
	 buf.blocks * buf.block_size >> 20,
	 (buf.blocks - buf.bavail) * buf.block_size >> 20,
	 buf.bavail * buf.block_size >> 20,
	 (buf.blocks - buf.bavail) * 100.0 / (buf.blocks ? buf.blocks : 1.0),
	 buf.read,
	 buf.write
	 );
}


int main(int argc, char **argv)
{
  glibtop_mountlist buf;

  glibtop_init();

  printf("%-30s %10s %10s %10s %5s %10s %10s\n",
	 "Filesystem", "Size", "Used", "Avail", "Use%", "Read", "Write");

  if (argc > 1) {
    while (*++argv)
      print_fsusage(*argv);
  } else {
    glibtop_mountentry *entries;
    size_t i;

    entries = glibtop_get_mountlist(&buf, TRUE);

    for(i = 0; i < buf.number; ++i)
      {
	print_fsusage(entries[i].mountdir);
      }

    g_free(entries);
  }

  glibtop_close();

  return 0;
}
