LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop.la

libgtop_la_SOURCES		= init.c open.c close.c command.c read.c \
				  read_data.c write.c lib.c parameter.c \
				  sysdeps.c errors.c glib-arrays.c

libgtop_la_LDFLAGS		= $(LT_VERSION_INFO)

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

EXTRA_DIST			= lib.pl

CLEANFILES			= $(BUILT_SOURCES)

