
AM_CPPFLAGS			= @AM_CPPFLAGS@

noinst_LTLIBRARIES		= libgtop_sysdeps-2.0.la libgtop_sysdeps_suid-2.0.la

libgtop_sysdeps_2_0_la_SOURCES  = siglist.c nosuid.c mem.c swap.c uptime.c \
				  procargs.c prockernel.c proclist.c procmap.c \
				  procmem.c procsegment.c procsignal.c disk.c \
				  proctime.c procuid.c procmem.c utils.c \
				  procstate.c sysinfo.c netlist.c procio.c


libgtop_sysdeps_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtop_sysdeps_suid_2_0_la_SOURCES = open.c close.c cpu.c loadavg.c \
				  msg_limits.c shm_limits.c sem_limits.c \
				  netload.c ppp.c utils.c

libgtop_sysdeps_suid_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_sysdeps_suid_2_0_la_LIBADD	= $(top_builddir)/sysdeps/common/libgtop_suid_common-2.0.la $(GLIB_LIBS)

libgtopinclude_HEADERS		= glibtop_server.h glibtop_machine.h glibtop_suid.h

libgtopincludedir		= $(includedir)/libgtop-2.0
