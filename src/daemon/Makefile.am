LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@ \
				  -DDEBUG -DREAL_DEBUG -D_BSD \
				  -DPARENT_DEBUG -DSLAVE_DEBUG \
				  -DGLIBTOP_DAEMON_SLAVE

if NEED_LIBGTOP
suid_sysdeps			= $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps_suid.la
suid_common			= $(top_builddir)/sysdeps/common/libgtop_suid_common.la
else
suid_sysdeps			=
suid_common			=
endif

bin_PROGRAMS			= libgtop_daemon @server_programs@

EXTRA_PROGRAMS			= libgtop_server

libgtop_daemon_SOURCES		= gnuserv.c slave.c main.c io.c version.c
libgtop_daemon_LDADD		= $(top_builddir)/lib/libgtop.la \
				  $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @sysdeps_suid_lib@ \
				  $(suid_sysdeps) $(suid_common)\
				  @INTLLIBS@ @LIBSUPPORT@ @X_LIBS@ -lXau
libgtop_daemon_LDFLAGS		= -static

libgtop_server_SOURCES		= server.c slave.c io.c version.c
libgtop_server_LDADD		= $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps_suid.la \
				  $(top_builddir)/sysdeps/common/libgtop_suid_common.la \
				  @LIBSUPPORT@
libgtop_server_LDFLAGS		= -static

