LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

if INODEDB
inodedb_SRCLIST			= inodedb.c
inodedb_DEFS			= -DSYSTEM_INODEDB=\"$(prefix)/var/libgtop/inodedb.db\"
else
inodedb_SRCLIST			=
inodedb_DEFS			=
endif

INCLUDES			= @INCLUDES@ $(inodedb_DEFS)

noinst_PROGRAMS			= test-backends

lib_LTLIBRARIES			= libgtop.la

noinst_LTLIBRARIES		= libgtop_server.la

libgtop_la_SOURCES		= errors.c glibtop-backend-info.c \
				  glibtop-client.c glibtop-server.c \
				  glibtop-backend.c glibtop-client-private.h \
				  glibtop-server-private.h xmalloc.c error.c \
				  $(inodedb_SRCLIST) $(BUILT_SOURCES)

libgtop_server_la_SOURCES	= error.c xmalloc.c

libgtop_la_LDFLAGS		= $(LT_VERSION_INFO)
libgtop_la_LIBADD		= $(GLIB_LIBS) $(XML_LIBS)

test_backends_SOURCES		= \
	test-backends.c

test_backends_LDADD		= libgtop.la

glibtopdir			= $(includedir)/glibtop

glibtop_HEADERS			= structures.h

BUILT_SOURCES			= lib.c structures.h

lib.c: lib.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/lib.pl < $(top_srcdir)/features.def > lib-t
	mv lib-t lib.c

structures.h: structures.pl $(top_builddir)/config.h \
	      $(top_srcdir)/features.def $(top_srcdir)/structures.def
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/structures.pl \
		$(top_srcdir)/features.def $(top_srcdir)/structures.def \
		structures.h > tmp-s
	mv tmp-s structures.h

CLEANFILES			= $(BUILT_SOURCES)

EXTRA_DIST			= lib.pl structures.pl

