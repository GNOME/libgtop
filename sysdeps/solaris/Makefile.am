LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop_sysdeps.la libgtop_sysdeps_suid.la

libgtop_sysdeps_la_SOURCES	= open.c close.c cpu.c mem.c \
				  safeio.c swap.c uptime.c loadavg.c \
				  proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c \
				  prockernel.c procsegment.c procargs.c \
				  procmap.c netload.c netinfo.c ppp.c \
				  procdata.c interfaces.c

libgtop_sysdeps_la_LDFLAGS	= $(LT_VERSION_INFO) -module -export-dynamic

libgtop_sysdeps_la_LIBADD	= @DL_LIB@

libgtop_sysdeps_suid_la_SOURCES	= open_suid.c close_suid.c \
				  shm_limits.c msg_limits.c sem_limits.c

libgtop_sysdeps_suid_la_LDFLAGS	= $(LT_VERSION_INFO) -module -export-dynamic


include_HEADERS			= glibtop_server.h glibtop_machine.h
noinst_HEADERS			= glibtop_private.h glibtop_suid.h

