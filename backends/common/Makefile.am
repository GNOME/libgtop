LINK		= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES	= @INCLUDES@

backenddir	= @LIBGTOP_BACKEND_DIR@

if LIBGTOP_USE_GMODULE
backendlibdir	= $(backenddir)
else
backendlibdir	= $(libdir)
endif

dynamic_ldflags	= @libgtop_dynamic_ldflags@

backend_DATA = \
	libgtop-common.backend

noinst_HEADERS = \
	backend-common.h

backendlib_LTLIBRARIES = \
	libgtop_backend_common.la

libgtop_backend_common_la_SOURCES = \
	backend-common.c		\
	marshal.c

libgtop_backend_common_la_LDFLAGS = \
	$(LT_VERSION_INFO) $(dynamic_ldflags)

libgtop_backend_common_la_LIBADD = \
	$(top_builddir)/sysdeps/common/libgtop_common.la	\
	$(top_builddir)/lib/libgtop_server.la

BUILT_SOURCES = \
	marshal.c

EXTRA_DIST = \
	marshal.pl	\
	libgtop-common.backend

CLEANFILES = $(BUILT_SOURCES)

marshal.c: marshal.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/marshal.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t marshal.c

