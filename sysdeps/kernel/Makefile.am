LINK = \
	$(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES = \
	@INCLUDES@

backenddir = \
	@LIBGTOP_BACKEND_DIR@

backend_LTLIBRARIES = \
	libgtop_backend_kernel.la

backend_DATA = \
	libgtop-kernel.backend

libgtop_backend_kernel_la_SOURCES = \
	cpu.c mem.c swap.c uptime.c loadavg.c \
	proclist.c procstate.c procuid.c \
	proctime.c procmem.c procsignal.c prockernel.c \
	procsegment.c procargs.c procmap.c netload.c \
	procdata.c backend-kernel.c marshal.c

libgtop_backend_kernel_la_LDFLAGS = \
	$(LT_VERSION_INFO)

noinst_HEADERS	= \
	glibtop_server.h glibtop_private.h

BUILT_SOURCES = \
	marshal.c \
	backend-kernel.h

EXTRA_DIST = \
	marshal.pl	\
	backend-kernel.pl \
	libgtop-sysdeps.backend

CLEANFILES = $(BUILT_SOURCES)

marshal.c: marshal.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/marshal.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t marshal.c

backend-kernel.h: backend-kernel.pl $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/backend-kernel.pl < $(top_srcdir)/features.def > tmp-t
	mv tmp-t backend-kernel.h

