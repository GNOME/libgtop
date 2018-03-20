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

#define PPRINT(DATA, FORMAT) printf("\t%4lu B %3lu " #DATA " = %" FORMAT "\n", \
(unsigned long) sizeof buf.DATA, (unsigned long) buf_offsetof(DATA), buf.DATA)

#define PPRINT_GUINT64(DATA) PPRINT(DATA, G_GUINT64_FORMAT)

#define PPRINT_FLAGS(DATA) printf("\t%4lu B %3lu " "flags" " = " "%#llx" "\n", \
(unsigned long) sizeof buf.DATA, (unsigned long) buf_offsetof(DATA), (unsigned long long)buf.DATA)

#define PPRINT_ARRAY(ARRAY, SIZE, FORMAT) do {				\
    size_t i;								\
    printf("\t%4lu B %3lu " #ARRAY "[%lu] = { ",			\
	   (unsigned long) sizeof buf.ARRAY,				\
	   (unsigned long) buf_offsetof(ARRAY),				\
	   (unsigned long) G_N_ELEMENTS(buf.ARRAY));			\
    for (i = 0; i < (SIZE); ++i) {					\
      if (!buf.ARRAY[i] && i < (SIZE - 1) && !buf.ARRAY[i + 1]) {	\
	do { i++; } while (i < SIZE && !buf.ARRAY[i]);			\
	printf("..., ");						\
      } else {								\
	printf(".%zd = %" FORMAT ", ", i, buf.ARRAY[i]);			\
      }									\
    }									\
    printf("}\n");							\
  } while(0)

#define PPRINT_ENTRY_ARRAY(ARRAY, SIZE) do { \
size_t i; \
printf("\t%4lu B " #ARRAY "[%lu/%lu] = { \n", \
(unsigned long) sizeof buf->ARRAY, (unsigned long)(SIZE),	\
(unsigned long) G_N_ELEMENTS(buf->ARRAY)); \
for(i = 0; i < SIZE; ++i) { \
  if (buf->ARRAY[i].values) {			\
    printf ("\t[ ");				\
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
  //PPRINT_FLAGS(flags);
  // PPRINT_GUINT64(ncpu);
  PPRINT_ENTRY_ARRAY(cpuinfo, buf->ncpu);
  FOOTER_PPRINT();
}

static void pprint_get_cpu(void)
{
  glibtop_cpu buf;

  glibtop_get_cpu(&buf);

  HEADER_PPRINT(glibtop_get_cpu);
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(total);
  PPRINT_GUINT64(user);
  PPRINT_GUINT64(nice);
  PPRINT_GUINT64(sys);
  PPRINT_GUINT64(idle);
  PPRINT_GUINT64(iowait);
  PPRINT_GUINT64(irq);
  PPRINT_GUINT64(softirq);
  PPRINT_GUINT64(frequency);
  PPRINT_ARRAY(xcpu_total, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_ARRAY(xcpu_user, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_ARRAY(xcpu_nice, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_ARRAY(xcpu_sys, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_ARRAY(xcpu_idle, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_ARRAY(xcpu_iowait, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_ARRAY(xcpu_irq, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_ARRAY(xcpu_softirq, GLIBTOP_NCPU, G_GUINT64_FORMAT);
  PPRINT_FLAGS(xcpu_flags);
  FOOTER_PPRINT();
}



static void pprint_get_fsusage(const char *mountpoint)
{
  glibtop_fsusage buf;

  glibtop_get_fsusage(&buf, mountpoint);

  HEADER_PPRINT(glibtop_get_fsusage);
  printf("pprint_get_fsusage (mountpoint = \"%s\"\n", mountpoint);
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(blocks);
  PPRINT_GUINT64(bfree);
  PPRINT_GUINT64(bavail);
  PPRINT_GUINT64(files);
  PPRINT_GUINT64(ffree);
  PPRINT(block_size, "u");
  PPRINT_GUINT64(read);
  PPRINT_GUINT64(write);
  FOOTER_PPRINT();
}



static void pprint_get_loadavg(void)
{
  glibtop_loadavg buf;

  glibtop_get_loadavg(&buf);

  HEADER_PPRINT(glibtop_get_loadavg);
  PPRINT_FLAGS(flags);
  PPRINT_ARRAY(loadavg, 3, "f");
  PPRINT_GUINT64(nr_running);
  PPRINT_GUINT64(nr_tasks);
  PPRINT_GUINT64(last_pid);
  FOOTER_PPRINT();
}



static void pprint_get_mem(void)
{
  glibtop_mem buf;

  glibtop_get_mem(&buf);

  HEADER_PPRINT(glibtop_get_mem);
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(total);
  PPRINT_GUINT64(used);
  PPRINT_GUINT64(free);
  PPRINT_GUINT64(shared);
  PPRINT_GUINT64(buffer);
  PPRINT_GUINT64(cached);
  PPRINT_GUINT64(user);
  PPRINT_GUINT64(locked);
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
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(number);
  PPRINT_GUINT64(total);
  PPRINT_GUINT64(size);

  printf("\t%4" G_GUINT64_FORMAT " B entries[%" G_GUINT64_FORMAT "] = \n\t\t{\n", buf.total, buf.number);

  for(i = 0; i < buf.number; ++i)
    {
      printf("\t\t .%lu = { .dev = %#llx,"
	     " .devname = \"%s\","
	     " .mountir = \"%s\","
	     " .type = \"%s\" }\n",
	     (unsigned long) i, (unsigned long long)entries[i].dev,
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
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(msgpool);
  PPRINT_GUINT64(msgmap);
  PPRINT_GUINT64(msgmax);
  PPRINT_GUINT64(msgmnb);
  PPRINT_GUINT64(msgmni);
  PPRINT_GUINT64(msgssz);
  PPRINT_GUINT64(msgtql);
  FOOTER_PPRINT();
}



static void pprint_get_netload(const char *iface)
{
  glibtop_netload buf;

  glibtop_get_netload(&buf, iface);

  HEADER_PPRINT(glibtop_get_netload);
  printf("glibtop_get_netload (iface = \"%s\")\n", iface);
  PPRINT_FLAGS(flags);
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
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(total);
  PPRINT_GUINT64(used);
  PPRINT_GUINT64(free);
  PPRINT_GUINT64(pagein);
  PPRINT_GUINT64(pageout);
  FOOTER_PPRINT();
}



static void pprint_get_uptime(void)
{
  glibtop_uptime buf;

  glibtop_get_uptime(&buf);

  HEADER_PPRINT(glibtop_get_uptime);
  PPRINT_FLAGS(flags);
  PPRINT(uptime, "f");
  PPRINT(idletime, "f");
  PPRINT_GUINT64(boot_time);
  FOOTER_PPRINT();
}




static void pprint_get_proc_kernel(pid_t pid)
{
  glibtop_proc_kernel buf;

  glibtop_get_proc_kernel(&buf, pid);

  HEADER_PPRINT(glibtop_get_proc_kernel);
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(k_flags);
  PPRINT_GUINT64(min_flt);
  PPRINT_GUINT64(maj_flt);
  PPRINT_GUINT64(cmin_flt);
  PPRINT_GUINT64(cmaj_flt);
  FOOTER_PPRINT();
}


static void pprint_get_proc_mem(pid_t pid)
{
  glibtop_proc_mem buf;

  glibtop_get_proc_mem(&buf, pid);

  HEADER_PPRINT(glibtop_get_proc_mem);
  PPRINT_FLAGS(flags);
  PPRINT_GUINT64(size);
  PPRINT_GUINT64(vsize);
  PPRINT_GUINT64(resident);
  PPRINT_GUINT64(share);
  PPRINT_GUINT64(rss);
  PPRINT_GUINT64(rss_rlim);
  FOOTER_PPRINT();
}


static void pprint_get_proc_affinity(pid_t pid)
{
  glibtop_proc_affinity buf;
  guint32 i;
  guint16* cpus;

  cpus = glibtop_get_proc_affinity(&buf, pid);

  HEADER_PPRINT(glibtop_get_proc_affinity);
  PPRINT_FLAGS(flags);
  PPRINT(number, "u");
  PPRINT(all, "d");

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
