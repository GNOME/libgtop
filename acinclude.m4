dnl This is just copied m4s from need-declaration.m4 gnome-fileutils.m4
dnl gnome-supprt-checks.m4 and a little from gnome-libgtop-check.m4
dnl and finally libgtop-sysdeps.m4

dnl
dnl LIBGTOP_CHECK_TYPE
dnl
dnl Improved version of AC_CHECK_TYPE which takes into account
dnl that we need to #include some other header files on some
dnl systems to get some types.

dnl AC_LIBGTOP_CHECK_TYPE(TYPE, DEFAULT)
AC_DEFUN([AC_LIBGTOP_CHECK_TYPE],
[AC_REQUIRE([AC_HEADER_STDC])dnl
AC_MSG_CHECKING(for $1)
AC_CACHE_VAL(ac_cv_type_$1,
[AC_EGREP_CPP(dnl
changequote(<<,>>)dnl
<<(^|[^a-zA-Z_0-9])$1[^a-zA-Z_0-9]>>dnl
changequote([,]), [#include <sys/types.h>
#if STDC_HEADERS
#include <stdlib.h>
#include <stddef.h>
#endif

/* For Tru64 */
#ifdef HAVE_SYS_BITYPES_H
#include <sys/bitypes.h>
#endif
], ac_cv_type_$1=yes, ac_cv_type_$1=no)])dnl
AC_MSG_RESULT($ac_cv_type_$1)
if test $ac_cv_type_$1 = no; then
  AC_DEFINE($1, $2)
fi
])

dnl
dnl GNOME_LIBGTOP_TYPES
dnl
dnl some typechecks for libgtop.
dnl

AC_DEFUN([GNOME_LIBGTOP_TYPES],
[
        AC_CHECK_HEADERS(sys/bitypes.h)
        AC_LIBGTOP_CHECK_TYPE(u_int64_t, unsigned long long int)
        AC_LIBGTOP_CHECK_TYPE(int64_t, signed long long int)
])

dnl See whether we need a declaration for a function.
dnl GCC_NEED_DECLARATION(FUNCTION [, EXTRA-HEADER-FILES])
AC_DEFUN([GCC_NEED_DECLARATION],
[AC_MSG_CHECKING([whether $1 must be declared])
AC_CACHE_VAL(gcc_cv_decl_needed_$1,
[AC_TRY_COMPILE([
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
$2],
[char *(*pfn) = (char *(*)) $1],
eval "gcc_cv_decl_needed_$1=no", eval "gcc_cv_decl_needed_$1=yes")])
if eval "test \"`echo '$gcc_cv_decl_needed_'$1`\" = yes"; then
  AC_MSG_RESULT(yes)
  gcc_need_declarations="$gcc_need_declarations $1"
  gcc_tr_decl=NEED_DECLARATION_`echo $1 | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
  AC_DEFINE_UNQUOTED($gcc_tr_decl)
else
  AC_MSG_RESULT(no)
fi
])dnl

dnl Check multiple functions to see whether each needs a declaration.
dnl GCC_NEED_DECLARATIONS(FUNCTION... [, EXTRA-HEADER-FILES])
AC_DEFUN([GCC_NEED_DECLARATIONS],
[for ac_func in $1
do
GCC_NEED_DECLARATION($ac_func, $2)
done
]
)

dnl
dnl GNOME_FILEUTILS_CHECKS
dnl
dnl checks that are needed for the diskusage applet.
dnl

AC_DEFUN([GNOME_FILEUTILS_CHECKS],
[
AC_CHECK_HEADERS(fcntl.h sys/param.h sys/statfs.h sys/fstyp.h \
mnttab.h mntent.h sys/statvfs.h sys/vfs.h sys/mount.h \
sys/filsys.h sys/fs_types.h sys/fs/s5param.h)

AC_CHECK_FUNCS(bcopy endgrent endpwent fchdir ftime ftruncate \
getcwd getmntinfo gettimeofday isascii lchown \
listmntent memcpy mkfifo strchr strerror strrchr vprintf)

dnl Set some defaults when cross-compiling

if test x$cross_compiling = xyes ; then
        case "$host_os" in
        linux*)
          fu_cv_sys_mounted_getmntent1=yes
          fu_cv_sys_stat_statfs2_bsize=yes
          ;;
        sunos*)
          fu_cv_sys_stat_statfs4=yes
          ;;
        freebsd*)
          fu_cv_sys_stat_statfs2_bsize=yes
          ;;
        osf*)
          fu_cv_sys_stat_statfs3_osf1=yes
          ;;
        esac
fi

# Determine how to get the list of mounted filesystems.
list_mounted_fs=

# If the getmntent function is available but not in the standard library,
# make sure LIBS contains -lsun (on Irix4) or -lseq (on PTX).
AC_FUNC_GETMNTENT

# This test must precede the ones for getmntent because Unicos-9 is
# reported to have the getmntent function, but its support is incompatible
# with other getmntent implementations.

# NOTE: Normally, I wouldn't use a check for system type as I've done for
# `CRAY' below since that goes against the whole autoconf philosophy.  But
# I think there is too great a chance that some non-Cray system has a
# function named listmntent to risk the false positive.

if test -z "$list_mounted_fs"; then
# Cray UNICOS 9
AC_MSG_CHECKING([for listmntent of Cray/Unicos-9])
AC_CACHE_VAL(fu_cv_sys_mounted_cray_listmntent,
[fu_cv_sys_mounted_cray_listmntent=no
AC_EGREP_CPP(yes,
[#ifdef _CRAY
yes
#endif
], [test $ac_cv_func_listmntent = yes \
&& fu_cv_sys_mounted_cray_listmntent=yes]
)
]
)
AC_MSG_RESULT($fu_cv_sys_mounted_cray_listmntent)
if test $fu_cv_sys_mounted_cray_listmntent = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_LISTMNTENT)
fi
fi

if test $ac_cv_func_getmntent = yes; then

# This system has the getmntent function.
# Determine whether it's the one-argument variant or the two-argument one.

if test -z "$list_mounted_fs"; then
# 4.3BSD, SunOS, HP-UX, Dynix, Irix
AC_MSG_CHECKING([for one-argument getmntent function])
AC_CACHE_VAL(fu_cv_sys_mounted_getmntent1,
[test $ac_cv_header_mntent_h = yes \
&& fu_cv_sys_mounted_getmntent1=yes \
|| fu_cv_sys_mounted_getmntent1=no])
AC_MSG_RESULT($fu_cv_sys_mounted_getmntent1)
if test $fu_cv_sys_mounted_getmntent1 = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_GETMNTENT1)
fi
fi

if test -z "$list_mounted_fs"; then
# SVR4
AC_MSG_CHECKING([for two-argument getmntent function])
AC_CACHE_VAL(fu_cv_sys_mounted_getmntent2,
[AC_EGREP_HEADER(getmntent, sys/mnttab.h,
fu_cv_sys_mounted_getmntent2=yes,
fu_cv_sys_mounted_getmntent2=no)])
AC_MSG_RESULT($fu_cv_sys_mounted_getmntent2)
if test $fu_cv_sys_mounted_getmntent2 = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_GETMNTENT2)
fi
fi

if test -z "$list_mounted_fs"; then
AC_MSG_ERROR([could not determine how to read list of mounted filesystems])
fi

fi

if test -z "$list_mounted_fs"; then
# DEC Alpha running OSF/1.
AC_MSG_CHECKING([for getfsstat function])
AC_CACHE_VAL(fu_cv_sys_mounted_getsstat,
[AC_TRY_LINK([
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/fs_types.h>],
[struct statfs *stats;
int numsys = getfsstat ((struct statfs *)0, 0L, MNT_WAIT); ],
fu_cv_sys_mounted_getsstat=yes,
fu_cv_sys_mounted_getsstat=no)])
AC_MSG_RESULT($fu_cv_sys_mounted_getsstat)
if test $fu_cv_sys_mounted_getsstat = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_GETFSSTAT)
fi
fi

if test -z "$list_mounted_fs"; then
# AIX.
AC_MSG_CHECKING([for mntctl function and struct vmount])
AC_CACHE_VAL(fu_cv_sys_mounted_vmount,
[AC_TRY_CPP([#include <fshelp.h>],
fu_cv_sys_mounted_vmount=yes,
fu_cv_sys_mounted_vmount=no)])
AC_MSG_RESULT($fu_cv_sys_mounted_vmount)
if test $fu_cv_sys_mounted_vmount = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_VMOUNT)
fi
fi

if test -z "$list_mounted_fs"; then
# SVR3
AC_MSG_CHECKING([for FIXME existence of three headers])
AC_CACHE_VAL(fu_cv_sys_mounted_fread_fstyp,
[AC_TRY_CPP([
#include <sys/statfs.h>
#include <sys/fstyp.h>
#include <mnttab.h>],
fu_cv_sys_mounted_fread_fstyp=yes,
fu_cv_sys_mounted_fread_fstyp=no)])
AC_MSG_RESULT($fu_cv_sys_mounted_fread_fstyp)
if test $fu_cv_sys_mounted_fread_fstyp = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_FREAD_FSTYP)
fi
fi

if test -z "$list_mounted_fs"; then
# 4.4BSD and DEC OSF/1.
AC_MSG_CHECKING([for getmntinfo function])
AC_CACHE_VAL(fu_cv_sys_mounted_getmntinfo,
[
ok=
if test $ac_cv_func_getmntinfo = yes; then
AC_EGREP_HEADER(f_type;, sys/mount.h,
ok=yes)
fi
test -n "$ok" \
&& fu_cv_sys_mounted_getmntinfo=yes \
|| fu_cv_sys_mounted_getmntinfo=no
])
AC_MSG_RESULT($fu_cv_sys_mounted_getmntinfo)
if test $fu_cv_sys_mounted_getmntinfo = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_GETMNTINFO)
fi
fi

# FIXME: add a test for netbsd-1.1 here

if test -z "$list_mounted_fs"; then
# Ultrix
AC_MSG_CHECKING([for getmnt function])
AC_CACHE_VAL(fu_cv_sys_mounted_getmnt,
[AC_TRY_CPP([
#include <sys/fs_types.h>
#include <sys/mount.h>],
fu_cv_sys_mounted_getmnt=yes,
fu_cv_sys_mounted_getmnt=no)])
AC_MSG_RESULT($fu_cv_sys_mounted_getmnt)
if test $fu_cv_sys_mounted_getmnt = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_GETMNT)
fi
fi

if test -z "$list_mounted_fs"; then
# SVR2
AC_MSG_CHECKING([whether it is possible to resort to fread on /etc/mnttab])
AC_CACHE_VAL(fu_cv_sys_mounted_fread,
[AC_TRY_CPP([#include <mnttab.h>],
fu_cv_sys_mounted_fread=yes,
fu_cv_sys_mounted_fread=no)])
AC_MSG_RESULT($fu_cv_sys_mounted_fread)
if test $fu_cv_sys_mounted_fread = yes; then
list_mounted_fs=found
AC_DEFINE(MOUNTED_FREAD)
fi
fi

if test -z "$list_mounted_fs"; then
AC_MSG_ERROR([could not determine how to read list of mounted filesystems])
# FIXME -- no need to abort building the whole package
# Cannot build mountlist.c or anything that needs its functions
fi

AC_CHECKING(how to get filesystem space usage)
space=no

# Perform only the link test since it seems there are no variants of the
# statvfs function.  This check is more than just AC_CHECK_FUNCS(statvfs)
# because that got a false positive on SCO OSR5.  Adding the declaration
# of a `struct statvfs' causes this test to fail (as it should) on such
# systems.  That system is reported to work fine with STAT_STATFS4 which
# is what it gets when this test fails.
if test $space = no; then
# SVR4
AC_CACHE_CHECK([statvfs function (SVR4)], fu_cv_sys_stat_statvfs,
[AC_TRY_LINK([#include <sys/types.h>
#include <sys/statvfs.h>],
[struct statvfs fsd; statvfs (0, &fsd);],
fu_cv_sys_stat_statvfs=yes,
fu_cv_sys_stat_statvfs=no)])
if test $fu_cv_sys_stat_statvfs = yes; then
space=yes
AC_DEFINE(STAT_STATVFS)
fi
fi

if test $space = no; then
# DEC Alpha running OSF/1
AC_MSG_CHECKING([for 3-argument statfs function (DEC OSF/1)])
AC_CACHE_VAL(fu_cv_sys_stat_statfs3_osf1,
[AC_TRY_RUN([
#include <sys/param.h>
#include <sys/types.h>
#include <sys/mount.h>
int main ()
{
struct statfs fsd;
fsd.f_fsize = 0;
return (statfs (".", &fsd, sizeof (struct statfs)));
}],
fu_cv_sys_stat_statfs3_osf1=yes,
fu_cv_sys_stat_statfs3_osf1=no,
fu_cv_sys_stat_statfs3_osf1=no)])
AC_MSG_RESULT($fu_cv_sys_stat_statfs3_osf1)
if test $fu_cv_sys_stat_statfs3_osf1 = yes; then
space=yes
AC_DEFINE(STAT_STATFS3_OSF1)
fi
fi

if test $space = no; then
# AIX
AC_MSG_CHECKING([for two-argument statfs with statfs.bsize dnl
member (AIX, 4.3BSD)])
AC_CACHE_VAL(fu_cv_sys_stat_statfs2_bsize,
[AC_TRY_RUN([
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif
int main ()
{
struct statfs fsd;
fsd.f_bsize = 0;
return (statfs (".", &fsd));
}],
fu_cv_sys_stat_statfs2_bsize=yes,
fu_cv_sys_stat_statfs2_bsize=no,
fu_cv_sys_stat_statfs2_bsize=no)])
AC_MSG_RESULT($fu_cv_sys_stat_statfs2_bsize)
if test $fu_cv_sys_stat_statfs2_bsize = yes; then
space=yes
AC_DEFINE(STAT_STATFS2_BSIZE)
fi
fi

if test $space = no; then
# SVR3
AC_MSG_CHECKING([for four-argument statfs (AIX-3.2.5, SVR3)])
AC_CACHE_VAL(fu_cv_sys_stat_statfs4,
[AC_TRY_RUN([#include <sys/types.h>
#include <sys/statfs.h>
int main ()
{
struct statfs fsd;
return (statfs (".", &fsd, sizeof fsd, 0));
}],
fu_cv_sys_stat_statfs4=yes,
fu_cv_sys_stat_statfs4=no,
fu_cv_sys_stat_statfs4=no)])
AC_MSG_RESULT($fu_cv_sys_stat_statfs4)
if test $fu_cv_sys_stat_statfs4 = yes; then
space=yes
AC_DEFINE(STAT_STATFS4)
fi
fi

if test $space = no; then
# 4.4BSD and NetBSD
AC_MSG_CHECKING([for two-argument statfs with statfs.fsize dnl
member (4.4BSD and NetBSD)])
AC_CACHE_VAL(fu_cv_sys_stat_statfs2_fsize,
[AC_TRY_RUN([#include <sys/types.h>
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
int main ()
{
struct statfs fsd;
fsd.f_fsize = 0;
return (statfs (".", &fsd));
}],
fu_cv_sys_stat_statfs2_fsize=yes,
fu_cv_sys_stat_statfs2_fsize=no,
fu_cv_sys_stat_statfs2_fsize=no)])
AC_MSG_RESULT($fu_cv_sys_stat_statfs2_fsize)
if test $fu_cv_sys_stat_statfs2_fsize = yes; then
space=yes
AC_DEFINE(STAT_STATFS2_FSIZE)
fi
fi

if test $space = no; then
# Ultrix
AC_MSG_CHECKING([for two-argument statfs with struct fs_data (Ultrix)])
AC_CACHE_VAL(fu_cv_sys_stat_fs_data,
[AC_TRY_RUN([#include <sys/types.h>
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#ifdef HAVE_SYS_FS_TYPES_H
#include <sys/fs_types.h>
#endif
int main ()
{
struct fs_data fsd;
/* Ultrix's statfs returns 1 for success,
0 for not mounted, -1 for failure.  */
return (statfs (".", &fsd) != 1);
}],
fu_cv_sys_stat_fs_data=yes,
fu_cv_sys_stat_fs_data=no,
fu_cv_sys_stat_fs_data=no)])
AC_MSG_RESULT($fu_cv_sys_stat_fs_data)
if test $fu_cv_sys_stat_fs_data = yes; then
space=yes
AC_DEFINE(STAT_STATFS2_FS_DATA)
fi
fi

if test $space = no; then
# SVR2
AC_TRY_CPP([#include <sys/filsys.h>],
AC_DEFINE(STAT_READ_FILSYS) space=yes)
fi

if test -n "$list_mounted_fs" && test $space != no; then
DF_PROG="df"
# AC_LIBOBJ(fsusage)
# AC_LIBOBJ(mountlist)
fi

# Check for SunOS statfs brokenness wrt partitions 2GB and larger.
# If <sys/vfs.h> exists and struct statfs has a member named f_spare,
# enable the work-around code in fsusage.c.
AC_MSG_CHECKING([for statfs that truncates block counts])
AC_CACHE_VAL(fu_cv_sys_truncating_statfs,
[AC_TRY_COMPILE([
#if !defined(sun) && !defined(__sun)
choke -- this is a workaround for a Sun-specific problem
#endif
#include <sys/types.h>
#include <sys/vfs.h>],
[struct statfs t; long c = *(t.f_spare);],
fu_cv_sys_truncating_statfs=yes,
fu_cv_sys_truncating_statfs=no,
)])
if test $fu_cv_sys_truncating_statfs = yes; then
AC_DEFINE(STATFS_TRUNCATES_BLOCK_COUNTS)
fi
AC_MSG_RESULT($fu_cv_sys_truncating_statfs)

AC_CHECKING(for AFS)
test -d /afs && AC_DEFINE(AFS)
])

dnl GNOME_SUPPORT_CHECKS
dnl    Check for various support functions needed by the standard
dnl    Gnome libraries.  Sets LIBOBJS, might define some macros.
dnl    This should only be used when building the Gnome libs; 
dnl    Gnome clients should not need this macro.
AC_DEFUN([GNOME_SUPPORT_CHECKS],[
  # we need an `awk' to build `gnomesupport.h'
  AC_REQUIRE([AC_PROG_AWK])

  # this should go away soon
  need_gnome_support=yes

  AC_CHECK_FUNCS(getopt_long,,
   [AC_LIBOBJ(getopt)
    AC_LIBOBJ(getopt1)])

  # for `scandir'
  AC_HEADER_DIRENT

  # copied from `configure.in' of `libiberty'
  vars="program_invocation_short_name program_invocation_name sys_errlist"
  for v in $vars; do
    AC_MSG_CHECKING([for $v])
    AC_CACHE_VAL(gnome_cv_var_$v,
      [AC_TRY_LINK([int *p;], [extern int $v; p = &$v;],
		   [eval "gnome_cv_var_$v=yes"],
		   [eval "gnome_cv_var_$v=no"])])
    if eval "test \"`echo '$gnome_cv_var_'$v`\" = yes"; then
      AC_MSG_RESULT(yes)
      n=HAVE_`echo $v | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
      AC_DEFINE_UNQUOTED($n)
    else
      AC_MSG_RESULT(no)
    fi
  done

  AC_REPLACE_FUNCS(memmove mkstemp scandir strcasecmp strerror strndup strnlen)
  AC_REPLACE_FUNCS(strtok_r strtod strtol strtoul vasprintf vsnprintf)

  AC_CHECK_FUNCS(realpath,,AC_LIBOBJ(canonicalize))

  # to include `error.c' error.c has some HAVE_* checks
  AC_CHECK_FUNCS(vprintf doprnt strerror_r)
  # stolen from am_func_error_at_line
  AC_CACHE_CHECK([for error_at_line], am_cv_lib_error_at_line,
  [AC_TRY_LINK([],[error_at_line(0, 0, "", 0, "");],
              am_cv_lib_error_at_line=yes,
              am_cv_lib_error_at_line=no)])
  if test $am_cv_lib_error_at_line = no; then
    AC_LIBOBJ(error)
  fi
  
  # This is required if we declare setreuid () and setregid ().
  AC_TYPE_UID_T

  # see if we need to declare some functions.  Solaris is notorious for
  # putting functions into the `libc' but not listing them in the headers
  AC_CHECK_HEADERS(string.h strings.h stdlib.h unistd.h dirent.h)
  GCC_NEED_DECLARATIONS(gethostname setreuid setregid getpagesize)
  GCC_NEED_DECLARATION(scandir,[
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
])

  AC_SUBST(need_gnome_support)

  AM_CONDITIONAL(BUILD_GNOME_SUPPORT, test "$need_gnome_support" = yes)
])


dnl This file is intended for use both internally in libgtop and in every program
dnl that wants to use it.
dnl
dnl It defines the following variables:
dnl
dnl * 'libgtop_sysdeps_dir'    - sysdeps dir for libgtop.
dnl * 'libgtop_use_machine_h'  - some of system dependend parts of libgtop provide
dnl                              their own header file. In this case we need to
dnl                              define 'HAVE_GLIBTOP_MACHINE_H'.
dnl * 'libgtop_need_server'    - is the server really needed? Defines 'NEED_LIBGTOP'
dnl                              if true; defines conditional 'NEED_LIBGTOP'.

AC_DEFUN([LIBGTOP_HACKER_TESTS],[
	AC_REQUIRE([AC_CANONICAL_HOST])

	AC_ARG_WITH(linux-table,
      [  --with-linux-table      Use the table () function from Martin Baulig],
	[linux_table="$withval"],[linux_table=auto])
	case "$host_os" in
	linux*)
	  if test $linux_table = yes ; then
	    AC_CHECK_HEADER(linux/table.h, linux_table=yes, linux_table=no)
	  elif test $linux_table = auto ; then
	    AC_MSG_CHECKING(for table function in Linux Kernel)
	    AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/unistd.h>
#include <linux/table.h>

#include <syscall.h>

static inline _syscall3 (int, table, int, type, union table *, tbl, const void *, param);

int
main (void)
{
	union table tbl;
	int ret;

	ret = table (TABLE_VERSION, NULL, NULL);

	if (ret == -1)
		exit (-errno);

	exit (ret < 1 ? ret : 0);
}
], linux_table=yes, linux_table=no, linux_table=no)
	    AC_MSG_RESULT($linux_table)
	  fi
	  if test $linux_table = yes ; then
	    AC_DEFINE(HAVE_LINUX_TABLE)
	  fi
	  ;;
	esac
])

AC_DEFUN([GNOME_LIBGTOP_SYSDEPS],[
	AC_REQUIRE([AC_CANONICAL_HOST])

	AC_SUBST(libgtop_sysdeps_dir)
	AC_SUBST(libgtop_use_machine_h)
	AC_SUBST(libgtop_need_server)

	AC_ARG_WITH(libgtop-examples,
	[  --with-libgtop-examples Build the libgtop examples (default=no)],[
	build_examples="$withval"], [build_examples=no])

	AM_CONDITIONAL(EXAMPLES, test x"$build_examples" = xyes)

	AC_ARG_ENABLE(hacker-mode,
	[  --enable-hacker-mode    Enable building of unstable sysdeps],
	[hacker_mode="$enableval"], [hacker_mode=no])

	AM_CONDITIONAL(HACKER_MODE, test x"$hacker_mode" = xyes)

	linux_table=auto
	if test x$hacker_mode = xyes ; then
	  LIBGTOP_HACKER_TESTS
	fi
	AM_CONDITIONAL(LINUX_TABLE, test $linux_table = yes)

	AC_ARG_WITH(libgtop-smp,
	[  --with-libgtop-smp      Enable SMP support (default-auto)],[
	libgtop_smp="$withval"],[libgtop_smp=auto])

	if test $libgtop_smp = auto ; then
	  AC_MSG_CHECKING(whether to enable SMP support)
	  case "$host_os" in
	  linux*)
	    libgtop_smp=yes
	    ;;
	  aix*)
	    libgtop_smp=yes
	    ;;
	  *)
	    libgtop_smp=no
	    ;;
	  esac
	  AC_MSG_RESULT($libgtop_smp)
	fi

	if test $libgtop_smp = yes ; then
	  AC_DEFINE(HAVE_LIBGTOP_SMP)
	fi

	AM_CONDITIONAL(LIBGTOP_SMP, test $libgtop_smp = yes)

	AC_MSG_CHECKING(for libgtop sysdeps directory)

	case "$host_os" in
	linux*)
	  if test x$linux_table = xyes ; then
	    libgtop_sysdeps_dir=kernel
	    libgtop_use_machine_h=no
	  else
	    libgtop_sysdeps_dir=linux
	    libgtop_use_machine_h=no
	    libgtop_have_sysinfo=yes
	  fi
	  libgtop_need_server=no
	  ;;
	freebsd*|netbsd*|openbsd*|bsdi*)
	  libgtop_sysdeps_dir=freebsd
	  libgtop_use_machine_h=yes
	  libgtop_need_server=yes
	  libgtop_postinstall='chgrp kmem $(bindir)/libgtop_server && chmod 2755 $(bindir)/libgtop_server'
	  ;;
	solaris*)
	  libgtop_sysdeps_dir=solaris
	  libgtop_use_machine_h=yes
	  libgtop_need_server=yes
	  libgtop_postinstall='chgrp sys $(bindir)/libgtop_server && chmod 2755 $(bindir)/libgtop_server'
	  ;;
        aix*)
	  libgtop_sysdeps_dir=aix
	  libgtop_use_machine_h=yes
	  libgtop_need_server=yes
	  libgtop_have_sysinfo=yes
	  libgtop_postinstall='chgrp system $(bindir)/libgtop_server && chmod g+s $(bindir)/libgtop_server2'
          ;;
	*)
	  if test x$hacker_mode = xyes ; then
	    case "$host_os" in
	    sunos4*)
	      #Please note that this port is obsolete and not working at
	      #all. It is only useful for people who want to fix it ... :-)
	      libgtop_sysdeps_dir=sun4
	      libgtop_use_machine_h=yes
	      libgtop_need_server=yes
	      ;;
	    osf*)
	      libgtop_sysdeps_dir=osf1
	      libgtop_use_machine_h=yes
	      libgtop_need_server=yes
	      ;;
	    *)
	      libgtop_sysdeps_dir=stub
	      libgtop_use_machine_h=no
	      libgtop_need_server=no
	      ;;
	    esac
	  else
	    libgtop_sysdeps_dir=stub
	    libgtop_use_machine_h=no
	    libgtop_need_server=no
	  fi
	  ;;
	esac

	test -z "$libgtop_postinstall" && libgtop_postinstall=:

	AC_MSG_RESULT($libgtop_sysdeps_dir)

	AC_SUBST(libgtop_sysdeps_dir)
	AC_SUBST(libgtop_postinstall)
	AC_SUBST(libgtop_have_sysinfo)

	case "$host_os" in
	*bsd*)
	  AC_CHECK_HEADERS(net/if_var.h)
	  AC_MSG_CHECKING([for I4B])
	  AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>

#include <net/if.h>
#include <net/if_types.h>

#ifdef HAVE_NET_IF_VAR_H
#include <net/if_var.h>
#endif

#include <net/netisr.h>
#include <net/route.h>

#if defined(__FreeBSD__) || defined(__NetBSD__)
#include <net/if_sppp.h>
#else
#include <i4b/sppp/if_sppp.h>
#endif
],[
	size_t size = sizeof (struct sppp);
], have_i4b=yes, have_i4b=no)
	  AC_MSG_RESULT($have_i4b)
	  if test x$have_i4b = xyes; then
	    AC_DEFINE(HAVE_I4B)
	    AC_MSG_CHECKING([for I4B accounting])
	    AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>

#include <net/if.h>
#include <net/if_types.h>

#ifdef HAVE_NET_IF_VAR_H
#include <net/if_var.h>
#endif

#include <net/netisr.h>
#include <net/route.h>

#if defined(__FreeBSD__) || defined(__NetBSD__)
#include <net/if_sppp.h>
#else
#include <i4b/sppp/if_sppp.h>
#endif

#include <machine/i4b_acct.h>
],[
	size_t size = sizeof (struct i4bisppp_softc);
], have_i4b_acct=yes, have_i4b_acct=no)
	    AC_MSG_RESULT($have_i4b_acct)
	    if test x$have_i4b_acct = xyes ; then
	      AC_DEFINE(HAVE_I4B_ACCT)
	    else
	      AC_WARN([
*** I4B accounting disabled - you won't get any PPP statistics.
*** Read "misc/i4b_acct.txt" in the LibGTop source directory
*** to see how to enable it.])
	    fi
	  fi
	  ;;
	linux*)
	  os_major_version=`uname -r | sed 's/-pre[[0-9]]*//' | \
	    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
	  os_minor_version=`uname -r | sed 's/-pre[[0-9]]*//' | \
	    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
	  os_micro_version=`uname -r | sed 's/-pre[[0-9]]*//' | \
	    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
	  os_version_expr="$os_major_version 65536 * $os_minor_version 256 * + $os_micro_version + p q"

	  AC_CHECK_HEADERS(linux/version.h, have_linux_version_h=yes,
	    have_linux_version_h=no)

	  if test x$have_linux_version_h = xyes ; then
	    version_code=`cat /usr/include/linux/version.h | \
	      grep \#define | grep LINUX_VERSION_CODE`
	    os_version_code=`echo $version_code | \
	      sed 's/^.*LINUX_VERSION_CODE[[ \t]]*\([[0-9]]*\).*$/\1/'`
	  else
	    os_version_code=`echo "$os_version_expr" | dc`
	  fi

	  AC_MSG_CHECKING(for Linux kernel version code)
	  AC_DEFINE_UNQUOTED(GLIBTOP_LINUX_VERSION_CODE, $os_version_code)
	  AC_MSG_RESULT($os_version_code)
	  ;;
	solaris*)
	  os_major_version=`uname -r | sed 's/\([[0-9]]*\).\([[0-9]]\)\.*\([[0-9]]*\)/\1/'`
	  os_minor_version=`uname -r | sed 's/\([[0-9]]*\).\([[0-9]]\)\.*\([[0-9]]*\)/\2/'`
	  os_micro_version=`uname -r | sed 's/\([[0-9]]*\).\([[0-9]]\)\.*\([[0-9]]*\)/\3/'`
	  test -z "$os_micro_version" && os_micro_version=0
	  os_version_expr="$os_major_version 100 * $os_minor_version 10 * + $os_micro_version + p q"
	  os_version_code=`echo "$os_version_expr" | dc`

	  AC_MSG_CHECKING(for Solaris release code)
	  AC_DEFINE_UNQUOTED(GLIBTOP_SOLARIS_RELEASE, $os_version_code)
	  AC_MSG_RESULT($os_version_code)
	  ;;
	esac

	AC_MSG_CHECKING(for machine.h in libgtop sysdeps dir)
	AC_MSG_RESULT($libgtop_use_machine_h)

	AC_MSG_CHECKING(whether we need libgtop)
	AC_MSG_RESULT($libgtop_need_server)

	if test x$libgtop_need_server = xyes ; then
	  AC_DEFINE(NEED_LIBGTOP)
	fi

	if test x$libgtop_use_machine_h = xyes ; then
	  AC_DEFINE(HAVE_GLIBTOP_MACHINE_H)
	fi

	AM_CONDITIONAL(NEED_LIBGTOP, test x$libgtop_need_server = xyes)
])

  
