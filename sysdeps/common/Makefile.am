LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_common.la libgtop_suid_common.la

libgtop_common_la_SOURCES	= sysdeps.c xmalloc.c error.c gnuslib.c

libgtop_suid_common_la_SOURCES	= xmalloc.c error.c
