
AM_CPPFLAGS			= @AM_CPPFLAGS@

noinst_LTLIBRARIES		= libgtop_sysdeps-2.0.la

libgtop_sysdeps_2_0_la_SOURCES	= open.c close.c siglist.c cpu.c disk.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c procargs.c procmap.c netload.c \
				  ppp.c netlist.c procopenfiles.c procaffinity.c \
				  procwd.c sysinfo.c

libgtop_sysdeps_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtopinclude_HEADERS		= glibtop_server.h
libgtopincludedir		= $(includedir)/libgtop-2.0
