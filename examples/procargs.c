#include <glibtop/procargs.h>

#include <stdlib.h>

#include <unistd.h>

static void show_args(pid_t pid, unsigned max_len)
{
  glibtop_proc_args buf;
  char ** const pargv = glibtop_get_proc_argv(&buf, pid, max_len);

  char **parg = pargv;

  printf("<%ld>\t", (long)pid);

  while(*parg)
    {
      printf("\"%s\" ", *parg);
      parg++;
    }

  putchar('\n');

  g_strfreev(pargv);
}


int main(int argc, char **argv)
{
  show_args(getpid(), 0);
  show_args(getpid(), 15);

  while(*++argv)
    {
      pid_t pid = strtol(*argv, NULL, 10);
      show_args(pid, 0);
      show_args(pid, 15);
    }

  return 0;
}

