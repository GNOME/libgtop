## Process this file with automake to produce Makefile.in

LINK			= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES		= @INCLUDES@

DEFS			= @DEFS@

noinst_PROGRAMS	= 	first second pprint procargs df netlist \
			  mountlist procmap netload sysdeps timings \
			  @static_targets@ @smp_examples@

EXTRA_PROGRAMS		= first_static second_static \
			  mountlist_static procmap_static \
			  smp smp_static \
			  netload_static sysdeps_static \
			  timings_static pprint_static procargs_static \
			  df_static netlist_static

first_SOURCES		= first.c
first_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

first_static_SOURCES	= $(first_SOURCES)
first_static_LDADD	= $(first_LDADD)
first_static_LDFLAGS	= -static

second_SOURCES		= second.c
second_LDADD		= $(top_builddir)/lib/libgtop-2.0.la -lm

second_static_SOURCES	= $(second_SOURCES)
second_static_LDADD	= $(second_LDADD)
second_static_LDFLAGS	= -static

procmap_SOURCES		= procmap.c
procmap_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

procmap_static_SOURCES	= $(procmap_SOURCES)
procmap_static_LDADD	= $(procmap_LDADD)
procmap_static_LDFLAGS	= -static

netload_SOURCES		= netload.c
netload_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

netload_static_SOURCES	= $(netload_SOURCES)
netload_static_LDADD	= $(netload_LDADD)
netload_static_LDFLAGS	= -static

sysdeps_SOURCES		= sysdeps.c
sysdeps_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

sysdeps_static_SOURCES	= $(sysdeps_SOURCES)
sysdeps_static_LDADD	= $(sysdeps_LDADD)
sysdeps_static_LDFLAGS	= -static


mountlist_SOURCES	= mountlist.c
mountlist_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

mountlist_static_SOURCES= $(mountlist_SOURCES)
mountlist_static_LDADD	= $(mountlist_LDADD)
mountlist_static_LDFLAGS= -static


smp_SOURCES		= smp.c
smp_LDADD		= $(top_builddir)/lib/libgtop-2.0.la -lm

smp_static_SOURCES	= $(smp_SOURCES)
smp_static_LDADD	= $(smp_LDADD)
smp_static_LDFLAGS	= -static

timings_SOURCES		= timings.c
timings_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

timings_static_SOURCES	= $(timings_SOURCES)
timings_static_LDADD	= $(timings_LDADD)
timings_static_LDFLAGS	= -static


pprint_SOURCES	        = pprint.c
pprint_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

pprint_static_SOURCES   = $(pprint_SOURCES)
pprint_static_LDADD	= $(pprint_LDADD)
pprint_static_LDFLAGS   = -static


procargs_SOURCES	  = procargs.c
procargs_LDADD		  = $(top_builddir)/lib/libgtop-2.0.la

procargs_static_SOURCES   = $(procargs_SOURCES)
procargs_static_LDADD	  = $(procargs_LDADD)
procargs_static_LDFLAGS   = -static


df_SOURCES	  = df.c
df_LDADD		  = $(top_builddir)/lib/libgtop-2.0.la

df_static_SOURCES   = $(df_SOURCES)
df_static_LDADD	  = $(df_LDADD)
df_static_LDFLAGS   = -static


netlist_SOURCES		= netlist.c
netlist_LDADD		= $(top_builddir)/lib/libgtop-2.0.la

netlist_static_SOURCES	= $(netlist_SOURCES)
netlist_static_LDADD	= $(netlist_LDADD)
netlist_static_LDFLAGS	= -static


