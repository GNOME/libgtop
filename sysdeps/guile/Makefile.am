if GLIBTOP_NAMES
SUBDIRS				= names
endif

DIST_SUBDIRS			= names

LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_guile.la

libgtop_guile_la_SOURCES	= boot.c sysdeps.c cpu.c mem.c swap.c uptime.c \
				  loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c

libgtop_guile_la_SOURCES	= guile.c

BUILT_SOURCES			= guile.c

guile.c: guile.awk $(top_builddir)/config.h $(top_builddir)/features.def
	$(AWK) -f $(srcdir)/guile.awk < $(top_builddir)/features.def > gnc-t
	mv gnc-t guile.c

EXTRA_DIST			= guile.awk

CLEANFILES			= guile.c
