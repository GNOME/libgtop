LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(includedir) -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_sysdeps.la

libgtop_sysdeps_la_SOURCES	= init.c open.c close.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c
libgtop_sysdeps_la_LIBADD	= -lkvm
