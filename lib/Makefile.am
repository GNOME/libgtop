LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop.la

libgtop_la_SOURCES		= init.c open.c close.c lib.c parameter.c \
				  sysdeps.c errors.c glib-arrays.c \
				  backend.c init-backends.c open-backend.c \
				  sysdeps-init.c

libgtop_la_LDFLAGS		= $(LT_VERSION_INFO)
libgtop_la_LIBADD		= $(LIBGTOP_XML_LIB)

glibtopdir			= $(includedir)/glibtop

glibtop_HEADERS			= structures.h

BUILT_SOURCES			= lib.c structures.h sysdeps-init.c

lib.c: lib.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/lib.pl < $(top_srcdir)/features.def > lib-t
	mv lib-t lib.c

structures.h: structures.pl $(top_builddir)/config.h \
	      $(top_srcdir)/features.def $(top_srcdir)/structures.def
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/structures.pl \
		$(top_srcdir)/features.def $(top_srcdir)/structures.def \
		structures.h > tmp-s
	mv tmp-s structures.h

sysdeps-init.c:
	rm -f sysdeps-init.c
	cp $(srcdir)/@sysdeps_init_file@ sysdeps-init.c

CLEANFILES			= $(BUILT_SOURCES)

EXTRA_DIST			= lib.pl structures.pl \
				  sysdeps-init-freebsd.c \
				  sysdeps-init-kernel.c \
				  sysdeps-init-linux.c \
				  sysdeps-init-solaris.c \
				  sysdeps-init-stub-suid.c \
				  sysdeps-init-stub.c

