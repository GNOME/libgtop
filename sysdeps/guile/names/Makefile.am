LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop_guile_names.la

BUILT_SOURCES			= guile-names.c guile-names.x

libgtop_guile_names_la_SOURCES	= $(BUILT_SOURCES)

libgtop_guile_names_la_LDFLAGS	= $(LT_VERSION_INFO)

Makefile: $(BUILT_SOURCES)

guile-names.c: guile-names.pl $(top_builddir)/config.h $(top_srcdir)/features.def
	$(PERL) $(srcdir)/guile-names.pl < $(top_srcdir)/features.def > gnc-t
	mv gnc-t guile-names.c

guile-names.x: guile-names.c
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $< > $@

EXTRA_DIST			= guile-names.pl

CLEANFILES			= guile-names.c guile-names.x

