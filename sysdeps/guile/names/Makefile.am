LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
				  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
				  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_guile_names.la

libgtop_guile_names_la_SOURCES	= guile-names.c

BUILT_SOURCES			= guile-names.c

guile-names.c: guile-names.awk $(top_builddir)/config.h $(top_builddir)/features.def
	$(AWK) -f $(srcdir)/guile-names.awk < $(top_builddir)/features.def > gnc-t
	mv gnc-t guile-names.c

EXTRA_DIST			= guile-names.awk

