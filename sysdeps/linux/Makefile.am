LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_sysdeps.la

libgtop_sysdeps_la_SOURCES	= open.c close.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c procmap.c siglist.c

libgtop_sysdeps_la_LDFLAGS	= $(LT_VERSION_INFO)

include_HEADERS			= glibtop_server.h glibtop_machine.h
