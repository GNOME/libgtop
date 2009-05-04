
INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop-2.0.la

libgtop_2_0_la_SOURCES		= init.c open.c close.c command.c read.c \
				  read_data.c write.c lib.c parameter.c \
				  sysdeps.c

libgtop_2_0_la_LDFLAGS		= $(LT_VERSION_INFO) -export-symbols $(srcdir)/libgtop.sym -no-undefined
libgtop_2_0_la_LIBADD		= $(GLIB_LIBS) $(top_builddir)/sysdeps/common/libgtop_common-2.0.la $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps-2.0.la $(sysdeps_suid_lib)

BUILT_SOURCES			= lib.c

lib.c: lib.pl $(top_builddir)/config.h $(top_srcdir)/features.def
	$(PERL) $(srcdir)/lib.pl < $(top_srcdir)/features.def > lib-t
	mv lib-t lib.c

EXTRA_DIST			= lib.pl libgtop.sym

CLEANFILES			= lib.c

