AM_CPPFLAGS			= @AM_CPPFLAGS@

noinst_LTLIBRARIES		= libgtop_sysdeps-2.0.la

EXTRA_DIST			= procmap_smaps.gperf procmap_smaps.c

libgtop_sysdeps_2_0_la_SOURCES	= open.c close.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c procargs.c procmap.c siglist.c \
				  sysinfo.c netload.c ppp.c glibtop_private.c \
				  mountlist.c  procaffinity.c \
				  fsusage.c netlist.c procopenfiles.c procwd.c

libgtop_sysdeps_2_0_la_LIBADD	=  @GLIB_LIBS@

# libgtop_sysdeps_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtopinclude_HEADERS		= glibtop_server.h glibtop_machine.h
libgtopincludedir		= $(includedir)/libgtop-2.0

noinst_HEADERS			= glibtop_private.h
