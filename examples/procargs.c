#include <glibtop/procargs.h>

#include <stdlib.h>

#include <unistd.h>

int main(int argc, char **argv)
{
  while(*++argv)
    {
      glibtop_proc_args buf;
      pid_t pid = strtol(*argv, NULL, 10);
      char ** const pargv = glibtop_get_proc_argv(&buf, pid, 20);

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
}

