## You need libtool 1.2 or newer for this Makefile.am to work.
##
## It _may_ work with an older version of libtool, but it also may fail.
## So if you get any undefined symbols here, please make sure you really
## have libtool 1.2 or better before reporting this as bug.
##
## You'll require libtool 1.2 for other parts of GNOME anyway.
##
## Get ftp://ftp.gnu.org/pub/gnu/libtool-1.2.tar.gz
## (or a newer version if it is available)
##
## Martin <martin@home-of-linux.org>
##

LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@ -D_BSD \
				  -DLIBGTOP_COMPILE_SYSTEM=\"`uname -s`\" \
				  -DLIBGTOP_COMPILE_RELEASE=\"`uname -r`\" \
				  -DLIBGTOP_COMPILE_MACHINE=\"`uname -m`\"

if NEED_LIBGTOP
suid_sysdeps			= $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps_suid.la
suid_common			= $(top_builddir)/sysdeps/common/libgtop_suid_common.la
else
suid_sysdeps			=
suid_common			=
endif

bin_PROGRAMS			= libgtop_daemon @server_programs@

EXTRA_PROGRAMS			= libgtop_server

libgtop_daemon_SOURCES		= gnuserv.c slave.c main.c io.c version.c \
				  daemon.h server_config.h
libgtop_daemon_LDADD		= $(top_builddir)/lib/libgtop.la \
				  $(top_builddir)/sysdeps/common/libgtop_common.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
				  @sysdeps_suid_lib@ \
				  $(suid_sysdeps) $(suid_common)\
				  $(GLIB_LIBS)\
				  @LIBSUPPORT@ @INTLLIBS@ @libs_xauth@

if ENABLE_STATIC
libgtop_daemon_LDFLAGS		= -static
endif

libgtop_server_SOURCES		= server.c slave.c io.c version.c daemon.h
libgtop_server_LDADD		= $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps_suid.la \
				  $(top_builddir)/sysdeps/common/libgtop_suid_common.la

if ENABLE_STATIC
libgtop_server_LDFLAGS		= -static
endif

EXTRA_DIST			= server_config.h.in server_config.pl

install-exec-hook:
	-@libgtop_postinstall@

