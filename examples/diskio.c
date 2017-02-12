#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glibtop.h>
#include <glibtop/procdiskio.h>

#include <stdlib.h>

#include <unistd.h>

static void show_diskio(pid_t pid)
{
  glibtop_proc_diskio diskio;

  glibtop_get_proc_diskio (&diskio, pid);

  printf("<%ld>\t", (long)pid);

  printf("rchar : %lu, wchar : %lu, read_bytes : %lu, write_bytes : %lu\n", diskio.rchar, diskio.wchar, diskio.rbytes, diskio.wbytes);

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

