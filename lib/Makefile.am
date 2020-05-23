
AM_CPPFLAGS			= @AM_CPPFLAGS@

lib_LTLIBRARIES			= libgtop-2.0.la

libgtop_2_0_la_SOURCES		= init.c open.c close.c command.c read.c \
				  read_data.c write.c lib.c parameter.c \
				  sysdeps.c boxed.c

libgtop_2_0_la_LDFLAGS		= $(LT_VERSION_INFO) -export-symbols $(srcdir)/libgtop.sym -no-undefined
libgtop_2_0_la_LIBADD		= $(GLIB_LIBS) $(top_builddir)/sysdeps/common/libgtop_common-2.0.la $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps-2.0.la $(sysdeps_suid_lib)

BUILT_SOURCES			= lib.c

lib.c: lib.pl $(top_builddir)/config.h $(top_srcdir)/features.def
	$(PERL) $(srcdir)/lib.pl < $(top_srcdir)/features.def > lib-t
	mv lib-t lib.c

EXTRA_DIST			= lib.pl libgtop.sym

CLEANFILES			= lib.c

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --accept-unprefixed --warn-all
INTROSPECTION_COMPILER_ARGS =

if HAVE_INTROSPECTION
introspection_sources = $(libgtop_2_0_la_SOURCES) lib.c ../glibtop.h ../libgtopconfig.h \
			../include/glibtop/close.h ../include/glibtop/loadavg.h ../include/glibtop/prockernel.h ../include/glibtop/procstate.h \
			../include/glibtop/sem_limits.h ../include/glibtop/uptime.h ../include/glibtop/command.h ../include/glibtop/mem.h ../include/glibtop/proclist.h \
			../include/glibtop/proctime.h ../include/glibtop/shm_limits.h ../include/glibtop/cpu.h ../include/glibtop/disk.h ../include/glibtop/msg_limits.h \
			../include/glibtop/procmem.h ../include/glibtop/procuid.h ../include/glibtop/swap.h \
			../include/glibtop/procsegment.h ../include/glibtop/sysdeps.h ../include/glibtop/global.h \
			../include/glibtop/procsignal.h ../include/glibtop/union.h ../include/glibtop/gnuserv.h \
			../include/glibtop/parameter.h ../include/glibtop/mountlist.h ../include/glibtop/fsusage.h ../include/glibtop/procmap.h ../include/glibtop/signal.h \
			../include/glibtop/sysinfo.h ../include/glibtop/ppp.h ../include/glibtop/procargs.h ../include/glibtop/netload.h \
			../include/glibtop/procwd.h ../include/glibtop/procaffinity.h ../include/glibtop/procio.h \
			../include/glibtop/netlist.h ../include/glibtop/procopenfiles.h ../include/glibtop/open.h

GTop-2.0.gir: libgtop-2.0.la
GTop_2_0_gir_AM_CPPFLAGS = GObject-2.0
GTop_2_0_gir_CFLAGS = $(AM_CPPFLAGS)
GTop_2_0_gir_LIBS = libgtop-2.0.la
GTop_2_0_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += GTop-2.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
