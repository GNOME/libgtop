LINK		= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES	= @INCLUDES@ -D_BSD \
		  -DLIBGTOP_COMPILE_SYSTEM=\"`uname -s`\" \
		  -DLIBGTOP_COMPILE_RELEASE=\"`uname -r`\" \
		  -DLIBGTOP_COMPILE_MACHINE=\"`uname -m`\"

noinst_HEADERS = \
	glibtop-backend-private.h

lib_LTLIBRARIES = \
	libgtop_backend_server.la

bin_PROGRAMS = \
	@server_programs@

EXTRA_PROGRAMS = \
	libgtop_server

libgtop_backend_server_la_SOURCES = \
	backend-server.c		\
	marshal.c			\
	command.c			\
	read.c				\
	read_data.c			\
	write.c

libgtop_backend_server_la_LDFLAGS = \
	$(LT_VERSION_INFO)

libgtop_server_SOURCES = \
	server.c		\
	server.h		\
	demarshal.c

libgtop_server_LDADD = \
	$(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps_suid.la \
	$(top_builddir)/sysdeps/common/libgtop_suid_common.la \
	@LIBSUPPORT@

if ENABLE_STATIC
libgtop_server_LDFLAGS = -static
endif

BUILT_SOURCES = \
	marshal.c	\
	demarshal.c

CLEANFILES = $(BUILT_SOURCES)

marshal.c: marshal.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/marshal.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t marshal.c

demarshal.c: demarshal.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/demarshal.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t demarshal.c

