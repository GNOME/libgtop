LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@ -DGTOP_SERVER=\""@LIBGTOP_SERVER@"\"

bin_PROGRAMS			= gnuserv

EXTRA_PROGRAMS			= gtop_daemon

gnuserv_SOURCES			= gnuserv.c main.c
gnuserv_LDADD			= $(top_builddir)/lib/libgtop.la \
				  $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @INTLLIBS@ @LIBSUPPORT@ @X_LIBS@ -lXau
gnuserv_LDFLAGS			= -static

gtop_daemon_SOURCES		= main.c inet.c loop.c output.c version.c
gtop_daemon_LDADD		= $(top_builddir)/lib/libgtop.la \
				  $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @INTLLIBS@ @LIBSUPPORT@
gtop_daemon_LDFLAGS		= -static
