LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@ -DGTOP_SERVER=\""@LIBGTOP_SERVER@"\"

lib_LTLIBRARIES			= libgtop.la

libgtop_la_SOURCES		= init.c open.c close.c command.c read.c read_data.c write.c lib.c

BUILT_SOURCES			= lib.c

lib.c: lib.awk $(top_builddir)/config.h $(top_builddir)/features.def
	$(AWK) -f $(srcdir)/lib.awk < $(top_builddir)/features.def > lib-t
	mv lib-t lib.c

EXTRA_DIST			= lib.awk

CLEANFILES			= lib.c

bin_PROGRAMS			= gnuclient
gnuclient_LDADD			= $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @INTLLIBS@ @LIBSUPPORT@ @X_LIBS@ -lXau
gnuclient_LDFLAGS			= -static
