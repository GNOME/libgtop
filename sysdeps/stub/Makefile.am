LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@ -D_IN_LIBGTOP_SERVER

noinst_LTLIBRARIES		= libgtop_sysdeps.la

libgtop_sysdeps_la_SOURCES	= open.c close.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c proccwd.c procargs.c procmap.c \
				  netload.c netinfo.c interfaces.c ppp.c

noinst_HEADERS			= glibtop_server_private.h

