LINK		= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES	= @INCLUDES@

backenddir	= @LIBGTOP_BACKEND_DIR@

backend_DATA = \
	libgtop-sysdeps.backend

noinst_HEADERS = \
	glibtop-backend-private.h

backend_LTLIBRARIES = \
	libgtop_backend_sysdeps.la

libgtop_backend_sysdeps_la_SOURCES = \
	backend-sysdeps.c		\
	marshal.c

libgtop_backend_sysdeps_la_LDFLAGS = \
	$(LT_VERSION_INFO) -export-dynamic

BUILT_SOURCES = \
	marshal.c

EXTRA_DIST = \
	marshal.pl	\
	libgtop-sysdeps.backend

CLEANFILES = $(BUILT_SOURCES)

marshal.c: marshal.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/marshal.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t marshal.c

