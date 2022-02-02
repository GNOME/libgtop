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

AM_CPPFLAGS			= $(LIBGTOP_CFLAGS) @AM_CPPFLAGS@ -D_BSD \
				  -DLIBGTOP_COMPILE_SYSTEM="\"`uname -s`\"" \
				  -DLIBGTOP_COMPILE_RELEASE="\"`uname -r`\"" \
				  -DLIBGTOP_COMPILE_VERSION="\"`uname -v`\"" \
				  -DLIBGTOP_COMPILE_MACHINE="\"`uname -m`\""

if LIBGTOP_NEED_SERVER
suid_sysdeps			= $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps_suid-2.0.la
suid_common			= $(top_builddir)/sysdeps/common/libgtop_suid_common-2.0.la
else
suid_sysdeps			=
suid_common			=
endif

libexec_PROGRAMS		= libgtop_daemon2 @server_programs@

EXTRA_PROGRAMS			= libgtop_server2

libgtop_daemon2_SOURCES		= gnuserv.c slave.c main.c io.c version.c \
				  daemon.h server_config.h
libgtop_daemon2_LDADD		= $(top_builddir)/lib/libgtop-2.0.la \
				  $(top_builddir)/sysdeps/common/libgtop_common-2.0.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps-2.0.la \
				  @sysdeps_suid_lib@ \
				  $(suid_sysdeps) $(suid_common) \
				  $(LIBGTOP_LIBS) \
				  @libs_xauth@

libgtop_server2_SOURCES		= server.c slave.c io.c version.c daemon.h
libgtop_server2_LDADD		= $(top_builddir)/lib/libgtop-2.0.la \
				  $(top_builddir)/sysdeps/common/libgtop_common-2.0.la \
				  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps-2.0.la \
				  @sysdeps_suid_lib@ \
				  $(suid_sysdeps) $(suid_common) \
				  $(LIBGTOP_LIBS) \
				  @libs_xauth@

EXTRA_DIST			= server_config.h.in server_config.pl

install-exec-hook:
	-test -n "$(DESTDIR)" || @libgtop_postinstall@

