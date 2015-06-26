#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glibtop.h>
#include <glibtop/sysinfo.h>

#include <glibtop/union.h>

#include <unistd.h>

#include <stdio.h>
#include <stddef.h>


#define buf_offsetof(MEMBER) ((ptrdiff_t)&(buf.MEMBER) - (ptrdiff_t)&buf)

#define HEADER_PPRINT(FUNC) printf("### " #FUNC " ###\nsizeof *buf = %lu B\n", \
(unsigned long) sizeof buf)

#define PPRINT(DATA, FORMAT) printf("\t%4lu B %3lu " #DATA " = " FORMAT "\n", \
(unsigned long) sizeof buf.DATA, (unsigned long) buf_offsetof(DATA), buf.DATA)

#define PPRINT_ARRAY(ARRAY, SIZE, FORMAT) do {				\
    size_t i;								\
    printf("\t%4lu B %3lu " #ARRAY "[%lu] = { ",			\
	   (unsigned long) sizeof buf.ARRAY,				\
	   (unsigned long) buf_offsetof(ARRAY),				\
	   (unsigned long) G_N_ELEMENTS(buf.ARRAY));			\
    for (i = 0; i < (SIZE); ++i) {					\
      printf(".%u = " FORMAT ", ", i, buf.ARRAY[i]);			\
      if (!buf.ARRAY[i] && i < (SIZE - 1) && !buf.ARRAY[i + 1]) {	\
	do { i++; } while (i < SIZE && !buf.ARRAY[i]);			\
	printf("..., ");						\
      }									\
    }									\
    printf("}\n");							\
  } while(0)

#define PPRINT_ENTRY_ARRAY(ARRAY, SIZE) do { \
size_t i; \
printf("\t%4lu B %3lu " #ARRAY "[%lu] = { ", \
(unsigned long) sizeof buf->ARRAY, 0,\
(unsigned long) G_N_ELEMENTS(buf->ARRAY)); \
for(i = 0; i < SIZE; ++i) { \
  if (buf->ARRAY[i].values) {			\
    printf ("[ ");				\
    PPRINT_HASHTABLE(buf->ARRAY[i].values);	\
    printf ("]\n");				\
  }						\
} \
printf("} "); \
} while(0)

#define PPRINT_HASHTABLE(HASHTABLE) do { \
g_hash_table_foreach (HASHTABLE, (GHFunc)pprint_hashtable_item, NULL); \
} while(0)

#define FOOTER_PPRINT() putchar('\n');

static void pprint_hashtable_item(gchar* key, gchar* value, gpointer user_data) 
{
  printf ("'%s': '%s', ", key, value);
}
static void pprint_get_sysinfo(void)
{
  const glibtop_sysinfo *buf;

  buf = glibtop_get_sysinfo();

  HEADER_PPRINT(glibtop_get_sysinfo);
  //PPRINT(flags, "%#llx");
  //PPRINT(ncpu, "%llu");
  PPRINT_ENTRY_ARRAY(cpuinfo, 4);
  FOOTER_PPRINT();
}

static void pprint_get_cpu(void)
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
  PPRINT(iowait, "%llu");
  PPRINT(irq, "%llu");
  PPRINT(softirq, "%llu");
  PPRINT(frequency, "%llu");
  PPRINT_ARRAY(xcpu_total, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_user, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_nice, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_sys, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_idle, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_iowait, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_irq, GLIBTOP_NCPU, "%llu");
  PPRINT_ARRAY(xcpu_softirq, GLIBTOP_NCPU, "%llu");
  PPRINT(xcpu_flags, "%llx");
  FOOTER_PPRINT();
}



static void pprint_get_fsusage(const char *mountpoint)
{
  glibtop_fsusage buf;

  glibtop_get_fsusage(&buf, mountpoint);

  HEADER_PPRINT(glibtop_get_fsusage);
  printf("pprint_get_fsusage (mountpoint = \"%s\"\n", mountpoint);
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



static void pprint_get_loadavg(void)
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



static void pprint_get_mem(void)
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

  HEADER_PPRINT(glibtop_get_mountlist);
  printf("glibtop_get_mountlist (allfs = %d)\n", allfs);
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
	     " .type = \"%s\" }\n",
	     (unsigned long) i, entries[i].dev,
	     entries[i].devname,
	     entries[i].mountdir,
	     entries[i].type);
    }

  printf("\t\t}\n");
  FOOTER_PPRINT();

  g_free(entries);
}



static void pprint_get_msg_limits(void)
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



static void pprint_get_netload(const char *iface)
{
  glibtop_netload buf;

  glibtop_get_netload(&buf, iface);

  HEADER_PPRINT(glibtop_get_netload);
  printf("glibtop_get_netload (iface = \"%s\")\n", iface);
  PPRINT(flags, "%#llx");
  FOOTER_PPRINT();
}


static void pprint_get_netlist(void)
{
  glibtop_netlist buf;
  char **devices;
  guint32 i;

  devices = glibtop_get_netlist(&buf);

  HEADER_PPRINT(glibtop_get_netlist);

  for(i = 0; i < buf.number; ++i)
  {
	  printf("\t%s\n", devices[i]);
  }

  FOOTER_PPRINT();

  g_strfreev(devices);
}



static void pprint_get_swap(void)
{
  glibtop_swap buf;

  glibtop_get_swap(&buf);

  HEADER_PPRINT(glibtop_get_swap);
  PPRINT(flags, "%#llx");
  PPRINT(total, "%llu");
  PPRINT(used, "%llu");
  PPRINT(free, "%llu");
  PPRINT(pagein, "%llu");
  PPRINT(pageout, "%llu");
  FOOTER_PPRINT();
}



static void pprint_get_uptime(void)
{
  glibtop_uptime buf;

  glibtop_get_uptime(&buf);

  HEADER_PPRINT(glibtop_get_uptime);
  PPRINT(flags, "%#llx");
  PPRINT(uptime, "%f");
  PPRINT(idletime, "%f");
  PPRINT(boot_time, "%llu");
  FOOTER_PPRINT();
}




static void pprint_get_proc_kernel(pid_t pid)
{
  glibtop_proc_kernel buf;

  glibtop_get_proc_kernel(&buf, pid);

  HEADER_PPRINT(glibtop_get_proc_kernel);
  PPRINT(flags, "%#llx");
  PPRINT(k_flags, "%llu");
  PPRINT(min_flt, "%llu");
  PPRINT(maj_flt, "%llu");
  PPRINT(cmin_flt, "%llu");
  PPRINT(cmaj_flt, "%llu");
  FOOTER_PPRINT();
}


static void pprint_get_proc_mem(pid_t pid)
{
  glibtop_proc_mem buf;

  glibtop_get_proc_mem(&buf, pid);

  HEADER_PPRINT(glibtop_get_proc_mem);
  PPRINT(flags, "%#llx");
  PPRINT(size, "%llu");
  PPRINT(vsize, "%llu");
  PPRINT(resident, "%llu");
  PPRINT(share, "%llu");
  PPRINT(rss, "%llu");
  PPRINT(rss_rlim, "%llu");
  FOOTER_PPRINT();
}


static void pprint_get_proc_affinity(pid_t pid)
{
  glibtop_proc_affinity buf;
  guint32 i;
  guint16* cpus;

  cpus = glibtop_get_proc_affinity(&buf, pid);

  HEADER_PPRINT(glibtop_get_proc_affinity);
  PPRINT(flags, "%#llx");
  PPRINT(number, "%u");
  PPRINT(all, "%d");

  printf("\taffinity=");
  for (i = 0; i < buf.number; i++) {
    printf("%d, ", cpus[i]);
  }
  putchar('\n');

  FOOTER_PPRINT();
}


int main()
{
  glibtop_init();

  pprint_get_sysinfo();
  pprint_get_cpu();

  pprint_get_fsusage("/");

  pprint_get_loadavg();

  pprint_get_mem();

  pprint_get_mountlist(TRUE);
  pprint_get_mountlist(FALSE);

  pprint_get_msg_limits();

  pprint_get_swap();

  pprint_get_netlist();

  pprint_get_netload("eth0");
  pprint_get_netload("ppp0");
  pprint_get_netload("<unknown>");
/* pprint_get_sysinfo(); */

  pprint_get_uptime();

  pprint_get_proc_kernel(getpid());
  pprint_get_proc_mem(getpid());
  pprint_get_proc_affinity(getpid());

  glibtop_close();

  return 0;
}
