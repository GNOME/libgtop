LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(includedir) -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@

bin_PROGRAMS			= gtop_server

gtop_server_SOURCES		= main.c output.c version.c
gtop_server_LDADD		= $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @INTLLIBS@ @LIBSUPPORT@
gtop_server_LDFLAGS		= -static

