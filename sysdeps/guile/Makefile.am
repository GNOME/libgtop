SUBDIRS				= names

LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

BUILT_SOURCES			= guile.c guile.x main.x

lib_LTLIBRARIES			= libgtop_guile.la

libgtop_guile_la_SOURCES	= main.c guile.h $(BUILT_SOURCES)

libgtop_guile_la_LDFLAGS	= $(LT_VERSION_INFO)

Makefile: $(BUILT_SOURCES)

guile.c structures.h:	guile.pl $(top_builddir)/config.h \
			$(top_srcdir)/features.def \
			$(top_srcdir)/structures.def \
			$(top_srcdir)/scripts/guile_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/guile.pl \
		$(top_srcdir)/features.def $(top_srcdir)/structures.def \
		structures.h > gnc-t
	mv gnc-t guile.c

guile.x: guile.c
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $< > $@

main.x: main.c
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $< > $@

EXTRA_DIST			= guile.pl

CLEANFILES			= $(BUILT_SOURCES)

