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
	libgtop-sysdeps.backend

noinst_HEADERS = \
	glibtop-backend-private.h

backendlib_LTLIBRARIES = \
	libgtop_backend_sysdeps.la

libgtop_backend_sysdeps_la_SOURCES = \
	backend-sysdeps.c		\
	marshal.c

libgtop_backend_sysdeps_la_LDFLAGS = \
	$(LT_VERSION_INFO) $(dynamic_ldflags)

libgtop_backend_sysdeps_la_LIBADD = \
	$(top_builddir)/sysdeps/$(sysdeps_dir)/libgtop_sysdeps.la
	$(top_builddir)/lib/libgtop_server.la

BUILT_SOURCES = \
	marshal.c

EXTRA_DIST = \
	marshal.pl	\
	libgtop-sysdeps.backend

CLEANFILES = $(BUILT_SOURCES)

marshal.c: marshal.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/marshal.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t marshal.c

