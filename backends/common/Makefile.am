LINK		= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES	= @INCLUDES@

noinst_HEADERS = \
	glibtop-backend-private.h

lib_LTLIBRARIES = \
	libgtop_backend_common.la

libgtop_backend_common_la_SOURCES = \
	backend-common.c		\
	marshal.c

libgtop_backend_common_la_LDFLAGS = \
	$(LT_VERSION_INFO)

BUILT_SOURCES = \
	marshal.c

EXTRA_DIST = \
	marshal.pl

CLEANFILES = $(BUILT_SOURCES)

marshal.c: marshal.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/marshal.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t marshal.c

