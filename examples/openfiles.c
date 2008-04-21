#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glibtop.h>
#include <glibtop/procopenfiles.h>

#include <stdlib.h>

#include <unistd.h>

static void show_open_files(pid_t pid)
{
  glibtop_proc_open_files buf;
  glibtop_open_files_entry *files;
  unsigned i;

  files = glibtop_get_proc_open_files(&buf, pid);

  printf("<%ld>\n", (long)pid);

  for(i = 0; i < buf.number; ++i)
  {
	  printf("\tfd = %d\t", files[i].fd);

	  switch(files[i].type)
	  {
	  case GLIBTOP_FILE_TYPE_FILE:
		  printf("file \"%s\"\n", files[i].info.file.name);
		  break;

	  case GLIBTOP_FILE_TYPE_PIPE:
		  printf("pipe\n");
		  break;

	  case GLIBTOP_FILE_TYPE_INETSOCKET:
		  printf("socket %s:%d\n", files[i].info.sock.dest_host, files[i].info.sock.dest_port);
		  break;

	  case GLIBTOP_FILE_TYPE_INET6SOCKET:
		  printf("socket [%s]:%d\n", files[i].info.sock.dest_host, files[i].info.sock.dest_port);
		  break;

	  case GLIBTOP_FILE_TYPE_LOCALSOCKET:
		  printf("localsocket %s\n", files[i].info.localsock.name);
		  break;

	  default:
		  printf("unknown type\n");
	  }
  }

  putchar('\n');

  g_free(files);
}


int main(int argc, char **argv)
{
  glibtop_init();

  show_open_files(getpid());

  while(*++argv)
    {
      pid_t pid = strtol(*argv, NULL, 10);
      show_open_files(pid);
    }

  glibtop_close();

  return 0;
}

