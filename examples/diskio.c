#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glibtop.h>
#include <glibtop/procio.h>

#include <stdlib.h>

#include <unistd.h>

static void show_diskio(pid_t pid)
{
  glibtop_proc_io io;

  glibtop_get_proc_io (&io, pid);

  printf("<%ld>\t", (long)pid);

  printf("flags: %08lx disk_rchar : %lu, disk_wchar : %lu, disk_read_bytes : %lu, disk_write_bytes : %lu\n", (unsigned long)io.flags, io.disk_rchar, io.disk_wchar, io.disk_rbytes, io.disk_wbytes);

}


int main(int argc, char **argv)
{
  glibtop_init();

  while(*++argv)
    {
      pid_t pid = strtol(*argv, NULL, 10);
      show_diskio(pid);
    }

  glibtop_close();

  return 0;
}

