#include <glibtop/procargs.h>

#include <stdlib.h>

#include <unistd.h>

static void show_args(pid_t pid)
{
  glibtop_proc_args buf;
  char ** const pargv = glibtop_get_proc_argv(&buf, pid, 0);

  char **parg = pargv;

  printf("<%ld>\t", (long)pid);

  while(*parg)
    {
      printf("\"%s\"\t", *parg);
      parg++;
    }

  putchar('\n');

  g_strfreev(pargv);
}


int main(int argc, char **argv)
{
  show_args(getpid());

  while(*++argv)
    {
      pid_t pid = strtol(*argv, NULL, 10);
      show_args(pid);
    }

  return 0;
}

