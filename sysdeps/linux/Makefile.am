LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

dynamic_ldflags			= @libgtop_dynamic_ldflags@

backenddir			= @LIBGTOP_BACKEND_DIR@

if LIBGTOP_USE_GMODULE
backendlibdir	= $(backenddir)
else
backendlibdir	= $(libdir)
endif

backendlib_LTLIBRARIES		= libgtop_sysdeps.la

libgtop_sysdeps_la_SOURCES	= open.c close.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c procargs.c procmap.c sysinfo.c \
				  interfaces.c netinfo.c netload.c ppp.c
libgtop_sysdeps_la_LIBADD	= @GLIB_LIBS@

libgtop_sysdeps_la_LDFLAGS	= $(LT_VERSION_INFO) $(dynamic_ldflags)

include_HEADERS			= glibtop_server.h glibtop_machine.h
