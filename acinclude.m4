dnl
dnl LIBGTOP_CHECK_TYPE
dnl
dnl Improved version of AC_CHECK_TYPE which takes into account
dnl that we need to #include some other header files on some
dnl systems to get some types.

dnl AC_LIBGTOP_CHECK_TYPE(TYPE, DEFAULT)
AC_DEFUN(AC_LIBGTOP_CHECK_TYPE,
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

dnl This is used internally for <glibtop-config.h>.

AC_DEFUN([GNOME_LIBGTOP_TYPES_PRIVATE],
[
	AC_CHECK_HEADERS(sys/bitypes.h)
	AC_LIBGTOP_CHECK_TYPE(u_int64_t, unsigned long long int)
	AC_LIBGTOP_CHECK_TYPE(int64_t, signed long long int)
	AC_LIBGTOP_CHECK_TYPE(u_int8_t, unsigned char)
	AC_LIBGTOP_CHECK_TYPE(int8_t, signed char)
])

dnl This file is intended for use both internally in libgtop and in every program
dnl that wants to use it.
dnl
dnl It defines the following variables:
dnl
dnl * 'libgtop_sysdeps_dir'    - sysdeps dir for libgtop.
dnl * 'libgtop_sysdeps_name'   - name of the sysdeps dir.
dnl * 'libgtop_use_machine_h'  - some of system dependend parts of libgtop provide
dnl                              their own header file. In this case we need to
dnl                              define 'HAVE_GLIBTOP_MACHINE_H'.
dnl * 'libgtop_need_server'    - is the server really needed? Defines 'NEED_LIBGTOP'
dnl                              if true; defines conditional 'NEED_LIBGTOP'.

AC_DEFUN([LIBGTOP_HACKER_TESTS],[
	AC_REQUIRE([AC_CANONICAL_HOST])

	case "$host_os" in
	linux*)
	  AC_ARG_WITH(linux-sysctl,
	  [  --with-linux-sysctl     Use the sysctl () interface from Martin Baulig],[
	  linux_sysctl="$withval"],[linux_sysctl=auto])
	  if test $linux_sysctl = yes ; then
	    AC_CHECK_HEADER(linux/libgtop.h, linux_sysctl=yes, linux_sysctl=no)
	  elif test $linux_sysctl = auto ; then
	    AC_MSG_CHECKING(for LibGTop sysctl support in Linux Kernel)
	    AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>

#include <sys/user.h>
#include <sys/sysctl.h>
#include <linux/libgtop.h>

#define SIZE(x) sizeof(x)/sizeof(x[0])

int name[2] = { CTL_LIBGTOP, LIBGTOP_VERSION };

int
main (void)
{
    unsigned version;
    size_t size = sizeof (version);

    if (sysctl (name, SIZE (name), &version, &size, NULL, 0))
	exit (1);
    else if (version < 1)
	exit (2);
    else
	exit (0);
}
], linux_sysctl=yes, linux_sysctl=no, linux_sysctl=no)
	    AC_MSG_RESULT($linux_sysctl)
	  fi
	  if test $linux_sysctl = yes ; then
	    AC_DEFINE(HAVE_LINUX_SYSCTL)
	  fi
	  AM_CONDITIONAL(LINUX_SYSCTL, test $linux_sysctl = yes)
	  ;;
	esac
])

AC_DEFUN([LIBGTOP_CHECK_SYSDEPS_DIR],[
	AC_MSG_CHECKING(for libgtop sysdeps directory)

	case "$host_os" in
	linux*)
	  libgtop_sysdeps_dir=linux
	  libgtop_use_machine_h=no
	  libgtop_have_sysinfo=no
	  libgtop_need_server=no
	  libgtop_have_sysdeps=yes
	  libgtop_dynlink_ok=yes
	  ;;
	freebsd*|netbsd*|openbsd*)
	  libgtop_sysdeps_dir=freebsd
	  libgtop_use_machine_h=yes
	  libgtop_need_server=yes
	  libgtop_have_sysdeps=no
	  libgtop_postinstall='chgrp kmem $(bindir)/libgtop_server && chmod 2755 $(bindir)/libgtop_server'
	  libgtop_dynlink_ok=yes
	  ;;
        bsdi*)
	  libgtop_sysdeps_dir=freebsd
	  libgtop_use_machine_h=yes
	  libgtop_need_server=yes
	  libgtop_have_sysdeps=no
	  libgtop_postinstall='chgrp kmem $(bindir)/libgtop_server && chmod 2755 $(bindir)/libgtop_server'
	  libgtop_dynlink_ok=yes
	  ;;
	solaris*)
	  libgtop_sysdeps_dir=solaris
	  libgtop_use_machine_h=yes
	  libgtop_need_server=yes
	  libgtop_have_sysdeps=yes
	  libgtop_postinstall='chgrp sys $(bindir)/libgtop_server && chmod 2755 $(bindir)/libgtop_server'
	  libgtop_dynlink_ok=no
	  ;;
	osf*)
	  libgtop_sysdeps_dir=osf1
	  libgtop_use_machine_h=yes
	  libgtop_need_server=yes
	  libgtop_have_sysdeps=yes
	  libgtop_dynlink_ok=yes
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
	      libgtop_have_sysdeps=no
	      libgtop_dynlink_ok=no
	      ;;
	    *)
	      libgtop_sysdeps_dir=stub
	      libgtop_use_machine_h=no
	      libgtop_need_server=no
	      libgtop_have_sysdeps=yes
	      libgtop_dynlink_ok=yes
	      ;;
	    esac
	  else
	    libgtop_sysdeps_dir=stub
	    libgtop_use_machine_h=no
	    libgtop_need_server=no
	    libgtop_have_sysdeps=yes
	    libgtop_dynlink_ok=yes
	  fi
	  ;;
	esac

	test -z "$libgtop_postinstall" && libgtop_postinstall=:

	AC_MSG_RESULT($libgtop_sysdeps_dir)
])

AC_DEFUN([GNOME_LIBGTOP_DYNLINK],[
	AC_MSG_CHECKING(whether to build gmodulized LibGTop)

	AC_ARG_ENABLE(modules,
	[  --disable-modules       Disables dynamic module loading],[
	if test x$withval = xyes; then 
	  with_modules=yes
        else
          with_modules=no
	fi
	],[with_modules=auto])

	dynworks=false
	deps=
	if test x$with_modules = xno; then
	  AC_MSG_RESULT(no)
	else
	  AC_MSG_RESULT(yes)
	fi

	AC_MSG_CHECKING(whether dynamic modules work)
	if test x$with_modules = xauto; then
	  oLIBS="$LIBS"
	  oCFLAGS="$CFLAGS"
	  LIBS="`glib-config --libs gmodule`"
	  CFLAGS="`glib-config --cflags gmodule`"
	  AC_TRY_RUN([
	    #include <glib.h>
	    #include <gmodule.h>
	    main ()
	    {
	    	if (g_module_supported ())
	    	    exit (0);
	    	else
	    	    exit (1);
	    }
	  ], dynworks=true, dynworks=false, dynworks=true)
	  LIBS="$oLIBS"
	  CFLAGS="$oCFLAGS"
    
	  dnl Now we check to see if our libtool supports shared lib deps
	  dnl (in a rather ugly way even)
	  if $dynworks; then
	    libgtop_libtool_config="${CONFIG_SHELL-/bin/sh} libtool --config"
	    libgtop_deplibs_check=`$libgtop_libtool_config | \
	      grep '^[[a-z_]]*check[[a-z_]]*_method=[['\''\"]]' | \
	        sed 's/.*[['\''"]]\(.*\)[['\''"]]$/\1/'`
	    if test "x$libgtop_deplibs_check" = "xnone" || \
	      test "x$libgtop_deplibs_check" = "xunknown" || \
	      test "x$libgtop_deplibs_check" = "x"; then
	        dynworks=false
	    fi
	  fi
	elif test x$with_modules = xyes; then
	  dynworks=true
	else
	  dynworks=false
	fi

	if $dynworks; then
	  AC_DEFINE(LIBGTOP_USE_GMODULE)
	  GMODULE_LIBS="`glib-config --libs gmodule`"
	  GMODULE_FLAGS="`glib-config --cflags gmodule`"
	  AC_SUBST(GMODULE_LIBS)
	  AC_SUBST(GMODULE_FLAGS)
	  libgtop_use_gmodule=yes
	  libgtop_dynamic_ldflags='-export-dynamic -module'
	else
	  libgtop_use_gmodule=no
	  libgtop_dynamic_ldflags=
	fi

	AC_MSG_RESULT($libgtop_use_gmodule)

	AC_SUBST(libgtop_dynamic_ldflags)
])

AC_DEFUN([GNOME_LIBGTOP_SYSDEPS],[
	AC_REQUIRE([AC_CANONICAL_HOST])

	AC_SUBST(libgtop_sysdeps_dir)
	AC_SUBST(libgtop_sysdeps_name)
	AC_SUBST(libgtop_use_machine_h)
	AC_SUBST(libgtop_need_server)
	AC_SUBST(libgtop_have_sysdeps)

	AC_ARG_ENABLE(hacker-mode,
	[  --enable-hacker-mode    Enable building of unstable sysdeps],
	[hacker_mode="$enableval"], [hacker_mode=no])

	AM_CONDITIONAL(HACKER_MODE, test x"$hacker_mode" = xyes)

	if test x$hacker_mode = xyes ; then
	  LIBGTOP_HACKER_TESTS
	  AC_ARG_WITH(sysdeps-dir,
	  [  --with-sysdeps-dir      Manually override sysdeps directory],[override_sysdeps_dir="$withval"],[override_sysdeps_dir=no])
	else
	  override_sysdeps_dir=no
	fi

	AC_ARG_WITH(libgtop-smp,
	[  --with-libgtop-smp      Enable SMP support (default-auto)],[
	libgtop_smp="$withval"],[libgtop_smp=auto])

	if test $libgtop_smp = auto ; then
	  AC_MSG_CHECKING(whether to enable SMP support)
	  case "$host_os" in
	  linux*)
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

	if test x$override_sysdeps_dir = xno ; then
	  LIBGTOP_CHECK_SYSDEPS_DIR
	else
	  case $override_sysdeps_dir in
	  stub)
	    libgtop_sysdeps_dir=stub
	    libgtop_use_machine_h=no
	    libgtop_have_sysinfo=no
	    libgtop_need_server=no
	    libgtop_have_sysdeps=yes
	    libgtop_dynlink_ok=yes
	    ;;
	  stub_suid)
	    libgtop_sysdeps_dir=stub_suid
	    libgtop_use_machine_h=yes
	    libgtop_have_sysinfo=no
	    libgtop_need_server=yes
	    libgtop_have_sysdeps=no
	    libgtop_dynlink_ok=yes
	    ;;
	  linux|kernel)
	    libgtop_sysdeps_dir=linux
	    libgtop_use_machine_h=no
	    libgtop_have_sysinfo=no
	    libgtop_need_server=no
	    libgtop_have_sysdeps=yes
	    libgtop_dynlink_ok=yes
	    ;;
	  bsd)
	    libgtop_sysdeps_dir=freebsd
	    libgtop_use_machine_h=yes
	    libgtop_need_server=yes
	    libgtop_have_sysdeps=no
	    libgtop_postinstall='chgrp kmem $(bindir)/libgtop_server && chmod 2755 $(bindir)/libgtop_server'
	    libgtop_dynlink_ok=yes
	    ;;
	  solaris)
	    libgtop_sysdeps_dir=solaris
	    libgtop_use_machine_h=yes
	    libgtop_need_server=yes
	    libgtop_have_sysdeps=yes
	    libgtop_postinstall='chgrp sys $(bindir)/libgtop_server && chmod 2755 $(bindir)/libgtop_server'
	    libgtop_dynlink_ok=no
	    ;;
	  osf)
	    libgtop_sysdeps_dir=osf1
	    libgtop_use_machine_h=yes
	    libgtop_need_server=yes
	    libgtop_have_sysdeps=yes
	    libgtop_dynlink_ok=yes
	    ;;
	  *)
	    AC_MSG_ERROR(Invalid value for --with-sysdeps-dir)
	    ;;
	  esac
	fi

	AC_MSG_CHECKING(for libgtop sysdeps name)
	libgtop_sysdeps_name=`echo $libgtop_sysdeps_dir | sed -e 's/_/-/g'`
	AC_MSG_RESULT($libgtop_sysdeps_name)

	AC_MSG_CHECKING(whether to use dynamic linking in LibGtop)
	AC_MSG_RESULT($libgtop_dynlink_ok)

	AC_SUBST(libgtop_sysdeps_dir)
	AC_SUBST(libgtop_postinstall)
	AC_SUBST(libgtop_have_sysinfo)

	case "$host_os" in
	*bsd*)
	  AC_CHECK_HEADERS(net/if_var.h osreldate.h)
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

	AC_MSG_CHECKING(for Modem lockfile)
	AC_ARG_WITH(modem-lockfile,
	  [  --with-modem-lockfile   Modem lockfile for PPP support],[libgtop_modem_lockfile="$withval"],
	  [if test -f /dev/modem ; then
	     libgtop_modem_lockfile='/var/lock/LCK..modem'
	   else
	     libgtop_modem_lockfile='/var/lock/LCK..ttyS%u'
	   fi])
	AC_MSG_RESULT($libgtop_modem_lockfile)
	AC_DEFINE_UNQUOTED(LIBGTOP_MODEM_LOCKFILE, "$libgtop_modem_lockfile")

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

	if test x$libgtop_dynlink_ok = xyes ; then
	  GNOME_LIBGTOP_DYNLINK
	else
	  libgtop_use_gmodule=no
	fi

	AC_SUBST(libgtop_use_gmodule)

	if test x$libgtop_have_sysdeps = xyes ; then
	  AC_DEFINE(LIBGTOP_HAVE_SYSDEPS)
	fi

	if test x$libgtop_need_server = xyes ; then
	  AC_DEFINE(LIBGTOP_NEED_SERVER)
	fi

	AM_CONDITIONAL(NEED_LIBGTOP, test x$libgtop_need_server = xyes)
	AM_CONDITIONAL(HAVE_SYSDEPS, test x$libgtop_have_sysdeps = xyes)
	AM_CONDITIONAL(LIBGTOP_USE_GMODULE, test x$libgtop_use_gmodule = xyes)
])
