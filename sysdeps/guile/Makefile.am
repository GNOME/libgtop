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

libgtop_guile_la_SOURCES	= guile.c

BUILT_SOURCES			= guile.c

guile.c: guile.awk $(top_builddir)/config.h $(top_srcdir)/features.def
	$(AWK) -f $(srcdir)/guile.awk < $(top_srcdir)/features.def > gnc-t
	mv gnc-t guile.c

EXTRA_DIST			= guile.awk

CLEANFILES			= guile.c
