SUBDIRS				= names

LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_guile.la

libgtop_guile_la_SOURCES	= guile.c

BUILT_SOURCES			= guile.c

guile.c: guile.awk $(top_builddir)/config.h $(top_srcdir)/features.def
	$(AWK) -f $(srcdir)/guile.awk < $(top_srcdir)/features.def > gnc-t
	mv gnc-t guile.c

EXTRA_DIST			= guile.awk

CLEANFILES			= guile.c

