LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop_sysdeps.la libgtop_sysdeps_suid.la

libgtop_sysdeps_la_SOURCES	= open.c close.c siglist.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c ppp.c
libgtop_sysdeps_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_sysdeps_la_LIBADD	= -lmach

libgtop_sysdeps_suid_la_SOURCES	= open_suid.c close_suid.c proclist.c \
				  procstate.c procuid.c proctime.c procmem.c \
				  procsignal.c prockernel.c procsegment.c \
				  procmap.c procargs.c netload.c
libgtop_sysdeps_suid_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_sysdeps_suid_la_LIBADD	= -lmach

libgtopinclude_HEADERS		= glibtop_server.h glibtop_machine.h
libgtopincludedir		= $(includedir)/libgtop-1.0

noinst_HEADERS			= glibtop_suid.h
