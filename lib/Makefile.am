LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@ -DGTOP_SERVER=\""@LIBGTOP_SERVER@"\"

lib_LTLIBRARIES			= libgtop.la

libgtop_la_SOURCES		= init.c open.c close.c command.c read.c \
				  read_data.c write.c lib.c parameter.c \
				  sysdeps.c

BUILT_SOURCES			= lib.c

lib.c: lib.awk $(top_builddir)/config.h $(top_srcdir)/features.def
	$(AWK) -f $(srcdir)/lib.awk < $(top_srcdir)/features.def > lib-t
	mv lib-t lib.c

EXTRA_DIST			= lib.awk

CLEANFILES			= lib.c

