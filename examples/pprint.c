#include <glibtop.h>

#include <glibtop/cpu.h>
#include <glibtop/fsusage.h>
#include <glibtop/loadavg.h>
#include <glibtop/mem.h>
#include <glibtop/mountlist.h>
#include <glibtop/msg_limits.h>


#include <stdio.h>
#include <stddef.h>


#define buf_offsetof(MEMBER) ((ptrdiff_t)&(buf.MEMBER) - (ptrdiff_t)&buf)

#define HEADER_PPRINT(FUNC) printf(#FUNC "\tsizeof = %lu B\n", \
(unsigned long) sizeof buf)

#define PPRINT(DATA, FORMAT) printf("\t%4lu B %3lu " #DATA " = " FORMAT "\n", \
(unsigned long) sizeof buf.DATA, (unsigned long) buf_offsetof(DATA), buf.DATA)

#define PPRINT_ARRAY(ARRAY, SIZE, FORMAT) do { \
size_t i; \
printf("\t%4lu B %3lu " #ARRAY "[%lu] = { ", \
(unsigned long) sizeof buf.ARRAY, (unsigned long) buf_offsetof(ARRAY),\
(unsigned long) G_N_ELEMENTS(buf.ARRAY)); \
for(i = 0; i < (SIZE - 1); ++i) printf(".%u = " FORMAT ", ", i, buf.ARRAY[i]); \
printf(".%u = " FORMAT " }\n", SIZE - 1 , buf.ARRAY[SIZE - 1]); \
} while(0)

#define FOOTER_PPRINT() putchar('\n')



static void pprint_get_cpu()
{
  glibtop_cpu buf;

  glibtop_get_cpu(&buf);

  HEADER_PPRINT(glibtop_get_cpu);
  PPRINT(flags, "%#llx");
  PPRINT(total, "%llu");
  PPRINT(user, "%llu");
  PPRINT(nice, "%llu");
  PPRINT(sys, "%llu");
  PPRINT(idle, "%llu");
  PPRINT(frequency, "%llu");
  PPRINT_ARRAY(xcpu_total, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_user, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_nice, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_sys, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_idle, GLIBTOP_NCPU, "%llu");
  PPRINT(xcpu_flags, "%llx");
  FOOTER_PPRINT();
}



static void pprint_get_fsusage(const char *mountpoint)
{
  glibtop_fsusage buf;

  glibtop_get_fsusage(&buf, mountpoint);

  HEADER_PPRINT(glibtop_get_fsusage);
  PPRINT(flags, "%#llx");
  PPRINT(blocks, "%llu");
  PPRINT(bfree, "%llu");
  PPRINT(bavail, "%llu");
  PPRINT(files, "%llu");
  PPRINT(ffree, "%llu");
  PPRINT(block_size, "%u");
  PPRINT(read, "%llu");
  PPRINT(write, "%llu");
  FOOTER_PPRINT();
}



static void pprint_get_loadavg()
{
  glibtop_loadavg buf;

  glibtop_get_loadavg(&buf);

  HEADER_PPRINT(glibtop_get_loadavg);
  PPRINT(flags, "%#llx");
  PPRINT_ARRAY(loadavg, 3, "%f");
  PPRINT(nr_running, "%llu");
  PPRINT(nr_tasks, "%llu");
  PPRINT(last_pid, "%llu");
  FOOTER_PPRINT();
}



static void pprint_get_mem()
{
  glibtop_mem buf;

  glibtop_get_mem(&buf);

  HEADER_PPRINT(glibtop_get_mem);
  PPRINT(flags, "%#llx");
  PPRINT(total, "%llu");
  PPRINT(used, "%llu");
  PPRINT(free, "%llu");
  PPRINT(shared, "%llu");
  PPRINT(buffer, "%llu");
  PPRINT(cached, "%llu");
  PPRINT(user, "%llu");
  PPRINT(locked, "%llu");
  FOOTER_PPRINT();
}



static void pprint_get_mountlist(gboolean allfs)
{
  glibtop_mountlist buf;
  glibtop_mountentry *entries;
  size_t i;

  entries = glibtop_get_mountlist(&buf, allfs);

  HEADER_PPRINT(glibtop_get_mem);
  PPRINT(flags, "%#llx");
  PPRINT(number, "%llu");
  PPRINT(total, "%llu");
  PPRINT(size, "%llu");

  printf("\t%4llu B entries[%llu] = \n\t\t{\n", buf.total, buf.number);

  for(i = 0; i < buf.number; ++i)
    {
      printf("\t\t .%lu = { .dev = %#llx,"
	     " .devname = \"%s\","
	     " .mountir = \"%s\","
	     " .type - \"%s\" }\n",
	     (unsigned long) i, entries[i].dev,
	     entries[i].devname,
	     entries[i].mountdir,
	     entries[i].type);
    }

  printf("\t\t}\n");
  FOOTER_PPRINT();

  g_free(entries);
}



static void pprint_get_msg_limits()
{
  glibtop_msg_limits buf;

  glibtop_get_msg_limits(&buf);

  HEADER_PPRINT(glibtop_get_msg_limits);
  PPRINT(flags, "%#llx");
  PPRINT(msgpool, "%llu");
  PPRINT(msgmap, "%llu");
  PPRINT(msgmax, "%llu");
  PPRINT(msgmnb, "%llu");
  PPRINT(msgmni, "%llu");
  PPRINT(msgssz, "%llu");
  PPRINT(msgtql, "%llu");
  FOOTER_PPRINT();
}



int main()
{
  pprint_get_cpu();

  pprint_get_fsusage("/");

  pprint_get_loadavg();

  pprint_get_mem();

  pprint_get_mountlist(TRUE);
  pprint_get_mountlist(FALSE);

  pprint_get_msg_limits();

  return 0;
}
