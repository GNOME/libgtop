## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/sysdeps/$(sysdeps_dir) \
	-DGTOPLOCALEDIR=\"$(localedir)\" \
	$(GLIB_CFLAGS)

AM_LDFLAGS			= $(LIBGTOP_EXTRA_LIBS)

noinst_PROGRAMS	= 	first second pprint procargs df netlist \
			  mountlist procmap netload sysdeps timings \
			  openfiles smp proclist free wd affinity disk diskio sysinfo

first_SOURCES		= first.c
first_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

second_SOURCES		= second.c
second_LDADD		= $(top_builddir)/lib/libgtop-2.0.la -lm

procmap_SOURCES		= procmap.c
procmap_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

netload_SOURCES		= netload.c
netload_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

sysdeps_SOURCES		= sysdeps.c
sysdeps_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

mountlist_SOURCES	= mountlist.c
mountlist_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

smp_SOURCES		= smp.c
smp_LDADD		= $(top_builddir)/lib/libgtop-2.0.la -lm

disk_SOURCES		= disk.c
disk_LDADD		= $(top_builddir)/lib/libgtop-2.0.la -lm

timings_SOURCES		= timings.c
timings_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

pprint_SOURCES	        = pprint.c
pprint_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

procargs_SOURCES	  = procargs.c
procargs_LDADD		  = $(top_builddir)/lib/libgtop-2.0.la


df_SOURCES	  = df.c
df_LDADD		  = $(top_builddir)/lib/libgtop-2.0.la

netlist_SOURCES		= netlist.c
netlist_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

openfiles_SOURCES	= openfiles.c
openfiles_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

proclist_SOURCES	= proclist.c
proclist_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

wd_SOURCES	= wd.c
wd_LDADD	= $(top_builddir)/lib/libgtop-2.0.la

free_SOURCE	= mem.c
free_LDADD	= $(top_builddir)/lib/libgtop-2.0.la

affinity_SOURCES	= affinity.c
affinity_LDADD	= $(top_builddir)/lib/libgtop-2.0.la

diskio_SOURCES	  = diskio.c
diskio_LDADD	  = $(top_builddir)/lib/libgtop-2.0.la

sysinfo_SOURCES		= sysinfo.c
sysinfo_LDADD		= $(top_builddir)/lib/libgtop-2.0.la
