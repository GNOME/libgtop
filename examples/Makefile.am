## Process this file with automake to produce Makefile.in

LINK			= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES		= -I$(top_builddir) -I$(top_srcdir) @machine_incs@ \
			  -I$(top_srcdir)/include -I$(top_srcdir)/intl @GUILE_INCS@ \
			  -DGTOPLOCALEDIR=\"$(datadir)/locale\" -D_GNU_SOURCE

CFLAGS			= -Wall -W @CFLAGS@ -O0

DEFS			= @DEFS@

bin_PROGRAMS		= first first_static second second_static \
			  @guile_examples@

EXTRA_PROGRAMS		= third third_static

first_SOURCES		= first.c
first_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@ @libs_xauth@

first_static_SOURCES	= $(first_SOURCES)
first_static_LDADD	= $(first_LDADD)
first_static_LDFLAGS	= -static

second_SOURCES		= second.c
second_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@ @libs_xauth@

second_static_SOURCES	= $(second_SOURCES)
second_static_LDADD	= $(second_LDADD)
second_static_LDFLAGS	= -static

if GLIBTOP_NAMES
third_guile_names_LIBS	= $(top_builddir)/sysdeps/guile/names/libgtop_guile_names.la
third_names_LIBS	= $(top_builddir)/sysdeps/names/libgtop_names.la
endif

third_SOURCES		= third.c
third_LDADD		= $(top_builddir)/sysdeps/guile/libgtop_guile.la \
			  $(third_guile_names_LIBS) $(third_names_LIBS) \
			  $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @GUILE_LIBS@ @INTLLIBS@ @LIBSUPPORT@ @libs_xauth@

third_static_SOURCES	= $(third_SOURCES)
third_static_LDADD	= $(third_LDADD)
third_static_LDFLAGS	= -static
