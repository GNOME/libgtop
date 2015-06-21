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

  enum { SIZE, USED, FREE, AVAIL, READ, WRITE, MAX };
  char **a = g_new0(char*, MAX);

  glibtop_get_fsusage(&buf, mountpoint);


#define I(X) a[(X)]
  I(SIZE)  = g_format_size_full(buf.blocks * buf.block_size, G_FORMAT_SIZE_IEC_UNITS);
  I(USED)  = g_format_size_full((buf.blocks - buf.bavail)  * buf.block_size, G_FORMAT_SIZE_IEC_UNITS);
  I(FREE)  = g_format_size_full(buf.bfree * buf.block_size, G_FORMAT_SIZE_IEC_UNITS);
  I(AVAIL) = g_format_size_full(buf.bavail * buf.block_size, G_FORMAT_SIZE_IEC_UNITS);
  I(READ)  = g_format_size_full(buf.read, G_FORMAT_SIZE_IEC_UNITS);
  I(WRITE) = g_format_size_full(buf.write, G_FORMAT_SIZE_IEC_UNITS);

  printf("%#7x %10s %10s %10s %10s %5.1f%% %10s %10s %8lu %8lu %s\n",
    (unsigned)buf.flags,
    I(SIZE),
    I(USED),
    I(FREE),
    I(AVAIL),
    (buf.blocks - buf.bavail) * 100.0 / (buf.blocks ? buf.blocks : 1.0),
    I(READ),
    I(WRITE),
    (unsigned long)buf.files,
    (unsigned long)buf.ffree,
    mountpoint
    );

  g_strfreev(a);
}

int main(int argc, char **argv)
{
  glibtop_mountlist buf;

  glibtop_init();

  printf("%-6s %10s %10s %10s %10s %5s %10s %10s %10s %10s %s\n",
	 "(flags)", "Size", "Used", "Free", "Avail", "Use%", "Read", "Write", "Files", "Files Free", "Mountpoint");

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
