## Process this file with automake to produce Makefile.in

LINK			= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES		= @INCLUDES@

DEFS			= @DEFS@

# We install it in `libexec' since this will make it more easy
# to put the examples into an extra RPM package.

libgtopdir		= $(libexecdir)/libgtop

libgtop_PROGRAMS	= first second\
			  mountlist procmap netload sysdeps timings \
			  @static_targets@ @guile_examples@ @smp_examples@

EXTRA_PROGRAMS		= first_static second_static \
			  mountlist_static procmap_static \
			  third third_static smp smp_static \
			  netload_static sysdeps_static \
			  timings_static

first_SOURCES		= first.c
first_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@

first_static_SOURCES	= $(first_SOURCES)
first_static_LDADD	= $(first_LDADD)
first_static_LDFLAGS	= -static

second_SOURCES		= second.c
second_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@

second_static_SOURCES	= $(second_SOURCES)
second_static_LDADD	= $(second_LDADD)
second_static_LDFLAGS	= -static

procmap_SOURCES		= procmap.c
procmap_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@



procmap_static_SOURCES	= $(procmap_SOURCES)
procmap_static_LDADD	= $(procmap_LDADD)
procmap_static_LDFLAGS	= -static

netload_SOURCES		= netload.c
netload_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@

netload_static_SOURCES	= $(netload_SOURCES)
netload_static_LDADD	= $(netload_LDADD)
netload_static_LDFLAGS	= -static

sysdeps_SOURCES		= sysdeps.c
sysdeps_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@

sysdeps_static_SOURCES	= $(sysdeps_SOURCES)
sysdeps_static_LDADD	= $(sysdeps_LDADD)
sysdeps_static_LDFLAGS	= -static

third_guile_names_LIBS	= $(top_builddir)/sysdeps/guile/names/libgtop_guile_names.la
third_names_LIBS	= $(top_builddir)/sysdeps/names/libgtop_names.la

third_SOURCES		= third.c
third_LDADD		= $(top_builddir)/sysdeps/guile/libgtop_guile.la \
			  $(third_guile_names_LIBS) $(third_names_LIBS) \
			  $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @GUILE_LIBS@ @LIBSUPPORT@ @INTLLIBS@

third_static_SOURCES	= $(third_SOURCES)
third_static_LDADD	= $(third_LDADD)
third_static_LDFLAGS	= -static

mountlist_SOURCES	= mountlist.c
mountlist_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@

mountlist_static_SOURCES= $(mountlist_SOURCES)
mountlist_static_LDADD	= $(mountlist_LDADD)
mountlist_static_LDFLAGS= -static


smp_SOURCES		= smp.c
smp_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@

smp_static_SOURCES	= $(smp_SOURCES)
smp_static_LDADD	= $(smp_LDADD)
smp_static_LDFLAGS	= -static

timings_SOURCES		= timings.c
timings_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @LIBSUPPORT@ @INTLLIBS@

timings_static_SOURCES	= $(timings_SOURCES)
timings_static_LDADD	= $(timings_LDADD)
timings_static_LDFLAGS	= -static

