LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_guile_names.la

libgtop_guile_names_la_SOURCES	= guile-names.c

libgtop_guile_names_la_LDFLAGS	= $(LT_VERSION_INFO)

BUILT_SOURCES			= guile-names.c guile-names.x

guile-names.c: guile-names.awk $(top_builddir)/config.h $(top_srcdir)/features.def
	$(AWK) -f $(srcdir)/guile-names.awk < $(top_srcdir)/features.def > gnc-t
	mv gnc-t guile-names.c

guile-names.x: guile-names.c
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $< > $@

EXTRA_DIST			= guile-names.awk

CLEANFILES			= guile-names.c guile-names.x

