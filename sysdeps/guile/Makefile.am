SUBDIRS				= names

LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @GTOP_INCS@

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_guile.la

libgtop_guile_la_SOURCES	= boot.c sysdeps.c cpu.c mem.c swap.c uptime.c \
				  loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c
