SUBDIRS				= names

LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_guile.la

libgtop_guile_la_SOURCES	= guile.c

libgtop_guile_la_LDFLAGS	= $(LT_VERSION_INFO)

BUILT_SOURCES			= guile.c guile.x

guile.c: guile.awk $(top_builddir)/config.h $(top_srcdir)/features.def
	$(AWK) -f $(srcdir)/guile.awk < $(top_srcdir)/features.def > gnc-t
	mv gnc-t guile.c

guile.x: guile.c
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $< > $@

EXTRA_DIST			= guile.awk

CLEANFILES			= guile.c guile.x

