dnl
dnl Configure script for the Gnome library
dnl

m4_define([libgtop_major_version], [2])
m4_define([libgtop_minor_version], [41])
m4_define([libgtop_micro_version], [3])
m4_define([libgtop_version], [libgtop_major_version.libgtop_minor_version.libgtop_micro_version])

dnl  increment if the interface has additions, changes, removals.
m4_define([libgtop_current], [12])

dnl  increment any time the source changes; set to
dnl  0 if you increment CURRENT
m4_define([libgtop_revision], [0])

dnl  increment if any interfaces have been added; set to 0
dnl  if any interfaces have been removed. removal has
dnl  precedence over adding, so set to 0 if both happened.
m4_define([libgtop_age], [1])

# Increase each time you change the client/server protocol.
m4_define([libgtop_server_version], [5])

# Version code calculation
m4_define([libgtop_version_code], [m4_eval(libgtop_major_version * 1000000 + libgtop_minor_version * 1000 + libgtop_micro_version)])

AC_PREREQ(2.62)
AC_INIT([libgtop], [libgtop_version],
        [https://gitlab.gnome.org/GNOME/libgtop/-/issues/])
AC_CONFIG_SRCDIR(copyright.txt)
AC_CONFIG_HEADERS(config.h)

AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([-Wno-portability tar-ustar no-dist-gzip dist-xz])
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([m4])
GOBJECT_INTROSPECTION_CHECK([0.6.7])

AC_SUBST(ACLOCAL_AMFLAGS, [\${ACLOCAL_FLAGS}])

AC_SUBST(LIBGTOP_MAJOR_VERSION, [libgtop_major_version])
AC_SUBST(LIBGTOP_MINOR_VERSION, [libgtop_minor_version])
AC_SUBST(LIBGTOP_MICRO_VERSION, [libgtop_micro_version])
AC_SUBST(LIBGTOP_SERVER_VERSION, [libgtop_server_version])
AC_SUBST(LIBGTOP_VERSION, [libgtop_version])
AC_SUBST(LIBGTOP_VERSION_CODE, [libgtop_version_code])

# libtool versioning
AC_SUBST(LT_VERSION_INFO,
         ["-version-info libgtop_current:libgtop_revision:libgtop_age"])

ACLOCAL_FLAGS="$ACLOCAL_FLAGS -I ."

AC_ISC_POSIX
AC_PROG_CC
AC_STDC_HEADERS
dnl AC_ARG_PROGRAM

dnl We need this at some places.
libgtop_top_builddir=`pwd`
AC_SUBST(libgtop_top_builddir)

dnl We don't use `AC_PROG_AWK' since it checks for mawk first which
dnl does not work for libgtop.
AC_CHECK_PROGS(AWK, gawk awk, )
test -z "$AWK" && AC_MSG_ERROR([Sorry, you need a working awk interpreter.])

dnl Most people should have a working perl interpreter on their system
AC_CHECK_PROGS(PERL, perl5 perl)
test -z "$PERL" && AC_MSG_ERROR([You need to have a working perl interpreter.])

case "${host_os}" in
	linux*)
		AC_CHECK_TOOL(LSBLK,lsblk)
		AC_CHECK_TOOL(SED,sed)
		AC_CHECK_TOOL(TR,tr)
		;;
	*)
		;;
esac

AC_CHECK_TOOL(CC,gcc)
AC_CHECK_TOOL(RANLIB,ranlib)
AC_CHECK_TOOL(AS,as)
AC_CHECK_TOOL(AR,ar)

export CC CXX RANLIB LD AS AR

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

export cross_compiling

AM_PROG_LIBTOOL

AM_CONDITIONAL(CROSS_COMPILING, test "x$cross_compiling" = xyes)

GNOME_LIBGTOP_SYSDEPS
GNOME_LIBGTOP_TYPES

GLIB_REQUIRED=2.6.0
GOBJECT_REQUIRED=2.26.0

PKG_CHECK_MODULES(GLIB, glib-2.0 >= $GLIB_REQUIRED gobject-2.0 >= $GOBJECT_REQUIRED)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

AC_CHECK_HEADER(ifaddrs.h, [HAVE_IFADDRS_H=yes], [HAVE_IFADDRS_H=no])
if test x$HAVE_IFADDRS_H = xyes ; then
	AC_DEFINE(HAVE_IFADDRS_H, 1, [defined if you have ifaddrs.h])
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_ST_RDEV
AC_STRUCT_TM
AC_TYPE_UID_T

dnl Check sys/time.h
AC_CHECK_HEADERS([sys/time.h])

dnl For SunOS
AC_CHECK_TYPE(ssize_t, int)
AC_CHECK_HEADERS(memory.h)
AC_CHECK_HEADERS(string.h strings.h, break)
AC_REPLACE_FUNCS(strerror)

dnl dlopen() and dlsym()
DL_LIB=
AC_CHECK_FUNCS(dlopen,,[
  AC_CHECK_LIB(dl, dlopen, DL_LIB="-ldl", [
    AC_CHECK_LIB(dld, shl_load, DL_LIB="-ldld", [
      AC_CHECK_FUNCS(dlopen, DL_LIB="", DL_LIB="")
    ])
  ])
])
oLIBS="$LIBS"
LIBS="$LIBS $DL_LIB"
AC_CHECK_FUNCS(dlerror,,)
LIBS="$oLIBS"
AC_SUBST(DL_LIB)

dnl Linux
AC_CHECK_HEADER(linux/isdn.h,[HAVE_ISDN_H=yes],[HAVE_ISDN_H=no])
if test x$HAVE_ISDN_H = xyes ; then
	AC_DEFINE(HAVE_ISDN_H, 1, [defined if you have linux/isdn.h])
fi

dnl AIX
AC_CHECK_LIB(perfstat, vmgetinfo,
    AC_DEFINE(HAVE_VMGETINFO, 1,
        [Define to 1 if you have the 'vmgetinfo' function in libperfstat]))

dnl Solaris
case "$host_os" in
     solaris*)
     AC_CHECK_LIB(kstat, kstat_open)
     AC_CHECK_FUNCS(getloadavg swapctl)
     AC_CHECK_HEADERS(procfs.h sys/procfs.h, break)


    dnl Some versions of Solaris require -lelf for -lkvm
    case $host_os in
    solaris[[8-9]] | solaris2.[[0-9]].*)
    AC_CHECK_LIB(kvm, kvm_open,[
      LIBS="-lkvm $LIBS"
    ],[AC_MSG_CHECKING(for kvm_open in -lkvm with -lelf)
       AC_CACHE_VAL(ac_cv_lib_kvm_with_elf,
       [ac_save_LIBS="$LIBS"
        LIBS="-lkvm -lelf $LIBS"
        AC_TRY_LINK([char kvm_open();], [kvm_open()],
                    ac_cv_lib_kvm_with_elf=yes, ac_cv_lib_kvm_with_elf=no)
        LIBS="$ac_save_LIBS"
       ])
       if test "$ac_cv_lib_kvm_with_elf" = "yes"; then
         AC_MSG_RESULT(yes)
         LIBS="-lkvm -lelf $LIBS"
       else
         AC_MSG_RESULT(no)
       fi
    ])
     ;;
     esac
     ;;
esac

dnl For DEC OSF1
AC_CHECK_LIB(mach, vm_statistics)

dnl For some broken libc5 systems (Debian 1.3)
saved_CFLAGS=$CFLAGS
CFLAGS="$CFLAGS -D_GNU_SOURCE"
GCC_NEED_DECLARATION(program_invocation_name, [                           
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
])

CFLAGS=$saved_CFLAGS

AC_CHECK_FUNC(socket,
 [AC_CHECK_HEADER(netinet/in.h,
   [AC_CHECK_HEADER(arpa/inet.h, [
  AC_DEFINE(HAVE_SOCKETS, 1, [Define if BSD sockets are supported])
      AC_MSG_CHECKING("for sun_len member in struct sockaddr_un")
      AC_TRY_LINK([
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
      ],
      [static struct sockaddr_un x; x.sun_len = 1;],
      [AC_MSG_RESULT(yes); AC_DEFINE(HAVE_SOCKADDR_SUN_LEN, 1, [Define if struct sockaddr_un has a sun_len member])],
      [AC_MSG_RESULT(no)])])])])

dnl Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MMAP
AC_TYPE_SIGNAL
AC_FUNC_STRFTIME
AC_CHECK_FUNCS(getcwd gettimeofday getwd putenv sbrk strdup strtoul uname)

# FIXME: Remove AM_GNU_GETTEXT_VERSION once autoreconf supports REQUIRE_VERSION
AM_GNU_GETTEXT_VERSION([0.19.6])
AM_GNU_GETTEXT_REQUIRE_VERSION([0.19.6])
AM_GNU_GETTEXT([external])

GETTEXT_PACKAGE=libgtop-2.0
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
                   [Define to the gettext package used by the library])

AC_PATH_XTRA

dnl For Solaris
dnl Please don't move this before AC_PATH_XTRA
AC_CHECK_FUNC(gethostbyname,,[AC_CHECK_LIB(nsl,gethostbyname)])
AC_CHECK_FUNC(connect,,[AC_CHECK_LIB(socket,connect)])
AC_CHECK_FUNC(inet_aton,,[AC_CHECK_LIB(resolv,inet_aton)])

dnl The construct foo=`echo $w1 $w2 $w3` fails on some systems if $w1 = -e or -n
dnl So we use the following instead.
dnl XE_SPACE(var, words)
define([XE_SPACE],[
T=""
for W in $2; do if test -z "$T"; then T="$W"; else T="$T $W"; fi; done
$1="$T"
])dnl

dnl Autodetect Xauth
dnl -lXau is only used by gnuclient, so use a special variable for Xauth X libs
saved_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $X_CFLAGS"
test -z "$with_xauth" && { AC_CHECK_HEADER(X11/Xauth.h,          ,with_xauth=no) }
test -z "$with_xauth" && { AC_CHECK_LIB(Xau, XauGetAuthByAddr,[:],with_xauth=no, $X_LIBS) }
test -z "$with_xauth" && with_xauth=yes
if test "$with_xauth" = "yes"; then
  AC_DEFINE(HAVE_XAUTH, 1, [Define if Xauth is supported])
  XE_SPACE(libs_xauth, $X_LIBS -lXau)
fi
CPPFLAGS="$saved_CPPFLAGS"
AC_SUBST(libs_xauth)

dnl For diskusage stuff
GNOME_FILEUTILS_CHECKS

dnl Debugging
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],
                             [Turn on debugging @<:@default=no@:>@]),
              AC_DEFINE(LIBGTOP_ENABLE_DEBUG, 1, [Define to enable debugging]),
              AC_DEFINE(LIBGTOP_ENABLE_DEBUG, 0, [Define to disable debugging]))
AC_ARG_ENABLE(fatal-warnings,
              AS_HELP_STRING([--enable-fatal-warnings],
                             [Make all warnings fatal @<:@debug=no@:>@]),
              AC_DEFINE(LIBGTOP_FATAL_WARNINGS, 1,
                        [Define to enable fatal warnings]))

dnl These definitions are expanded in make.
LIBGTOP_LIBS='-L=$(libdir)'
LIBGTOP_INCS='-I=$(includedir)/libgtop-2.0'

if test x$libgtop_have_sysinfo = xyes ; then
  LIBGTOP_INCS="$LIBGTOP_INCS -DHAVE_LIBGTOP_SYSINFO"
fi

LIBGTOP_INCS="$LIBGTOP_INCS $GLIB_CFLAGS"

libgtop_save_prefix="$prefix"
libgtop_save_exec_prefix="$exec_prefix"
test "x$prefix" = xNONE && prefix=$ac_default_prefix
test "x$exec_prefix" = xNONE && exec_prefix=$prefix
LIBGTOP_BINDIR=`eval echo "${libexecdir}"`
LIBGTOP_SERVER=`eval echo "${libexecdir}/libgtop_server2"`
prefix="$libgtop_save_prefix"
exec_prefix="$libgtop_save_exec_prefix"

sysdeps_dir="$libgtop_sysdeps_dir"
AC_SUBST(sysdeps_dir)

if test -z "$LIBGTOP_EXTRA_LIBS" ; then
  LIBGTOP_EXTRA_LIBS="$libs_xauth"
else
  LIBGTOP_EXTRA_LIBS="$LIBGTOP_EXTRA_LIBS $libs_xauth"
fi

LIBGTOP_EXTRA_LIBS="$GLIB_LIBS $LIBGTOP_EXTRA_LIBS"

LIBGTOP_LIBS="$LIBGTOP_LIBS"

if test "$GCC" = "yes" ; then
        CFLAGS="\
	-Winline \
	-Wall \
	-Wchar-subscripts -Wmissing-declarations -Wmissing-prototypes \
	-Wnested-externs -Wpointer-arith \
	-Wcast-align -Wsign-compare \
	$CFLAGS"
fi

AM_CPPFLAGS="-I\$(top_builddir) -I\$(top_srcdir) -I\$(top_srcdir)/sysdeps/$sysdeps_dir -I\$(top_srcdir)/include $GLIB_CFLAGS $X_CFLAGS"

# Add various defines to the config.h file ...
AH_VERBATIM([_GNU_SOURCE],
[/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif])
AC_DEFINE([_IN_LIBGTOP], 1,
          [Note that we are building libgtop rather than using it])
AC_DEFINE([LIBGTOP_VERSION], ["libgtop_version"],
          [The libgtop version number])
AC_DEFINE([LIBGTOP_SERVER_VERSION], ["libgtop_server_version"],
          [The libgtop server version])
AC_DEFINE([LIBGTOP_VERSION_CODE], [libgtop_version_code],
          [The libgtop version number as an integer])
AC_DEFINE_UNQUOTED([LIBGTOP_SERVER], ["$LIBGTOP_SERVER"],
                   [The path to the libgtop server])

AC_SUBST(AM_CPPFLAGS)

AC_SUBST(LIBGTOP_EXTRA_LIBS)
AC_SUBST(GLIB_LIBS)
AC_SUBST(LIBGTOP_LIBS)
AC_SUBST(LIBGTOP_INCS)
AC_SUBST(LIBGTOP_BINDIR)
AC_SUBST(LIBGTOP_SERVER)

if test x$libgtop_need_server = xyes ; then
   sysdeps_suid_lib="\$(top_builddir)/sysdeps/\$(sysdeps_dir)/libgtop_sysdeps_suid-2.0.la"
   server_programs='libgtop_server2'
else
   sysdeps_suid_lib=
   server_programs=
fi
AC_SUBST(sysdeps_suid_lib)
AC_SUBST(server_programs)

GTK_DOC_CHECK(1.4)

AC_CONFIG_FILES([
Makefile
libgtop.spec
po/Makefile.in
misc/Makefile
include/Makefile
include/glibtop/Makefile
sysdeps/Makefile
sysdeps/common/Makefile
sysdeps/stub/Makefile
sysdeps/stub_suid/Makefile
sysdeps/sun4/Makefile
sysdeps/osf1/Makefile
sysdeps/linux/Makefile
sysdeps/freebsd/Makefile
sysdeps/solaris/Makefile
sysdeps/aix/Makefile
sysdeps/bsd/Makefile
sysdeps/darwin/Makefile
sysdeps/cygwin/Makefile
sysdeps/openbsd/Makefile
src/Makefile
src/daemon/Makefile
lib/Makefile
examples/Makefile
doc/Makefile
doc/reference/Makefile
doc/reference/version.xml
libgtop-2.0.pc
libgtopconfig.h
])

AC_OUTPUT
