LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop_names.la

libgtop_names_la_SOURCES	= cpu.c mem.c swap.c uptime.c loadavg.c \
				  shm_limits.c msg_limits.c sem_limits.c \
				  proclist.c sysdeps.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c \
				  prockernel.c procsegment.c fsusage.c \
				  mountlist.c procargs.c procmap.c netload.c \
				  ppp.c

libgtop_names_la_LDFLAGS	= $(LT_VERSION_INFO)

