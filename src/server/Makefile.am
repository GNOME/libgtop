LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @GTOP_INCS@

CFLAGS				= -Wall -W @CFLAGS@

bin_PROGRAMS			= gtop_server

gtop_server_SOURCES		= main.c output.c version.c
gtop_server_LDADD		= $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @INTLLIBS@ @LIBSUPPORT@
gtop_server_LDFLAGS		= -static

