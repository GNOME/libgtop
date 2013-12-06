include $top_srcdir/glib-tap.mk

noinst_PROGRAMS = test

TESTS = test

test_LDADD		= $(top_builddir)/lib/libgtop-2.0.la \
				  $(top_builddir)/sysdeps/common/libgtop_common-2.0.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps-2.0.la \
				  @sysdeps_suid_lib@ \
				  $(suid_sysdeps) $(suid_common) \
				  $(LIBGTOP_LIBS) \
				  @libs_xauth@

test_CPPFLAGS = \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)	\
	-I$(top_srcdir)/include/glibtop \
	$(NULL)

test_SOURCES = \
    cpuinfo-tests.c
