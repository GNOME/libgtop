LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@ \
				  -DDEBUG -DREAL_DEBUG -D_BSD \
				  -DPARENT_DEBUG -DSLAVE_DEBUG \
				  -DGLIBTOP_DAEMON_SLAVE

bin_PROGRAMS			= libgtop_daemon

if NEED_LIBGTOP
suid_sysdeps			= $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps_suid.la
else
suid_sysdeps			=
endif

libgtop_daemon_SOURCES		= gnuserv.c slave.c main.c io.c
libgtop_daemon_LDADD		= $(top_builddir)/lib/libgtop.la \
				  $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @sysdeps_suid_lib@ \
				  @INTLLIBS@ @LIBSUPPORT@ @X_LIBS@ -lXau
libgtop_daemon_LDFLAGS		= -static
