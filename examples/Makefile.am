## Process this file with automake to produce Makefile.in

LINK			= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES		= @GTOP_INCS@

CFLAGS			= -Wall -W @CFLAGS@

DEFS			= @DEFS@

bin_PROGRAMS		= first first_static first_linux \
			  second second_static second_linux \
			  @guile_examples@

EXTRA_PROGRAMS		= third third_static third_linux

first_SOURCES		= first.c
first_LDADD		= $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/lib/libgtop.la \
			  @INTLLIBS@ @LIBSUPPORT@

first_static_SOURCES	= $(first_SOURCES)
first_static_LDADD	= $(first_LDADD)
first_static_LDFLAGS	= -static

first_linux_SOURCES	= $(first_SOURCES)
first_linux_LDADD	= $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@
first_linux_LDFLAGS	= -static

second_SOURCES		= second.c
second_LDADD		= $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/lib/libgtop.la \
			  @INTLLIBS@ @LIBSUPPORT@

second_static_SOURCES	= $(second_SOURCES)
second_static_LDADD	= $(second_LDADD)
second_static_LDFLAGS	= -static

second_linux_SOURCES	= $(second_SOURCES)
second_linux_LDADD	= $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@
second_linux_LDFLAGS	= -static

if GLIBTOP_NAMES
third_guile_names_LIBS	= $(top_builddir)/sysdeps/guile/names/libgtop_guile_names.la
third_names_LIBS	= $(top_builddir)/sysdeps/names/libgtop_names.la
endif

third_SOURCES		= third.c
third_LDADD		= $(top_builddir)/sysdeps/guile/libgtop_guile.la \
			  $(third_guile_names_LIBS) $(third_names_LIBS) \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/lib/libgtop.la \
			  @GUILE_LIBS@ @INTLLIBS@ @LIBSUPPORT@

third_static_SOURCES	= $(third_SOURCES)
third_static_LDADD	= $(third_LDADD)
third_static_LDFLAGS	= -static

third_linux_SOURCES	= $(third_SOURCES)
third_linux_LDADD	= $(top_builddir)/sysdeps/guile/libgtop_guile.la \
			  $(third_guile_names_LIBS) $(third_names_LIBS) \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @GUILE_LIBS@ @INTLLIBS@ @LIBSUPPORT@
third_linux_LDFLAGS	= -static
