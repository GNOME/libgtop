## Process this file with automake to produce Makefile.in

LINK			= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS			= -Wall -W @CFLAGS@ -O0

DEFS			= @DEFS@

bin_PROGRAMS		= first first_static second second_static \
			  mountlist mountlist_static \
			  procmap procmap_static \
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

procmap_SOURCES		= procmap.c
procmap_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@ @libs_xauth@

procmap_static_SOURCES	= $(procmap_SOURCES)
procmap_static_LDADD	= $(procmap_LDADD)
procmap_static_LDFLAGS	= -static



third_guile_names_LIBS	= $(top_builddir)/sysdeps/guile/names/libgtop_guile_names.la
third_names_LIBS	= $(top_builddir)/sysdeps/names/libgtop_names.la

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

mountlist_SOURCES	= mountlist.c
mountlist_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@ @libs_xauth@

mountlist_static_SOURCES= $(mountlist_SOURCES)
mountlist_static_LDADD	= $(mountlist_LDADD)
mountlist_static_LDFLAGS= -static

