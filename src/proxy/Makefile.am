LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@ -DGTOP_SERVER=\""@LIBGTOP_SERVER@"\"

bin_PROGRAMS			= gtop_proxy

gtop_proxy_SOURCES		= main.c output.c version.c
gtop_proxy_LDADD		= $(top_builddir)/lib/libgtop.la \
				  $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @INTLLIBS@ @LIBSUPPORT@
gtop_proxy_LDFLAGS		= -static
