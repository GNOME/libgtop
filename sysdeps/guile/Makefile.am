SUBDIRS				= names

LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

BUILT_SOURCES			= guile.c guile.x

lib_LTLIBRARIES			= libgtop_guile.la

libgtop_guile_la_SOURCES	= $(BUILT_SOURCES)

libgtop_guile_la_LDFLAGS	= $(LT_VERSION_INFO)

Makefile: $(BUILT_SOURCES)

guile.c: guile.pl $(top_builddir)/config.h $(top_srcdir)/features.def
	$(PERL) $(srcdir)/guile.pl < $(top_srcdir)/features.def > gnc-t
	mv gnc-t guile.c

guile.x: guile.c
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $< > $@

EXTRA_DIST			= guile.pl

CLEANFILES			= guile.c guile.x

