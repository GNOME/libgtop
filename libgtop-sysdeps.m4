dnl This file is intended for use both internally in libgtop and in every program
dnl that wants to use it.
dnl
dnl It defines the following variables:
dnl
dnl * 'libgtop_sysdeps_dir'    - sysdeps dir for libgtop.
dnl * 'libgtop_need_server'    - is the server really needed? Defines 'LIBGTOP_NEED_SERVER'
dnl                              if true; defines conditional 'LIBGTOP_NEED_SERVER'.

AC_DEFUN([GNOME_LIBGTOP_SYSDEPS],[
	AC_REQUIRE([AC_CANONICAL_HOST])

	AC_SUBST(libgtop_sysdeps_dir)
	AC_SUBST(libgtop_need_server)

	AC_ARG_WITH(examples,
	AS_HELP_STRING([--with-examples],
	[Build the libgtop examples @<:@default=yes@:>@]),[
	build_examples="$withval"], [build_examples=yes])

	AM_CONDITIONAL(EXAMPLES, test x"$build_examples" = xyes)

	AC_ARG_ENABLE(hacker-mode,
	AS_HELP_STRING([--enable-hacker-mode],
	[Enable building of unstable sysdeps]),
	[hacker_mode="$enableval"], [hacker_mode=no])

	AM_CONDITIONAL(HACKER_MODE, test x"$hacker_mode" = xyes)

	AC_MSG_CHECKING(for libgtop sysdeps directory)

	case "$host_os" in
	linux*)
	  libgtop_sysdeps_dir=linux
	  libgtop_have_sysinfo=yes
	  libgtop_need_server=yes
	  libgtop_sysdeps_private_mountlist=yes
	  libgtop_sysdeps_private_fsusage=yes
	  libgtop_postinstall='chown root $(DESTDIR)$(libexecdir)/libgtop_server2 && chmod 4755 $(DESTDIR)$(libexecdir)/libgtop_server2'
	  ;;
	netbsd*|bsdi*)
	  libgtop_sysdeps_dir=bsd
	  libgtop_need_server=yes
	  libgtop_postinstall='chgrp kmem $(DESTDIR)$(libexecdir)/libgtop_server2 && chmod 2755 $(DESTDIR)$(libexecdir)/libgtop_server2'
	  ;;
	openbsd*)
	  libgtop_sysdeps_dir=openbsd
	  libgtop_need_server=yes
	  libgtop_sysdeps_private_mountlist=yes
	  libgtop_sysdeps_private_fsusage=yes
	  libgtop_postinstall='chgrp kmem $(DESTDIR)$(libexecdir)/libgtop_server2 && chmod 2555 $(DESTDIR)$(libexecdir)/libgtop_server2'
	  ;;
	freebsd*|kfreebsd*)
	  libgtop_sysdeps_dir=freebsd
	  libgtop_need_server=yes
	  libgtop_sysdeps_private_mountlist=yes
	  libgtop_sysdeps_private_fsusage=yes
	  libgtop_postinstall='chgrp kmem $(DESTDIR)$(libexecdir)/libgtop_server2 && chmod 2755 $(DESTDIR)$(libexecdir)/libgtop_server2'
	  ;;
	solaris*)
	  libgtop_sysdeps_dir=solaris
	  libgtop_need_server=yes
	  libgtop_postinstall='chgrp sys $(DESTDIR)$(libexecdir)/libgtop_server && chmod 2755 $(DESTDIR)$(libexecdir)/libgtop_server'
	  ;;
	aix*)
	  libgtop_sysdeps_dir=aix
	  libgtop_need_server=yes
	  libgtop_have_sysinfo=yes
	  libgtop_postinstall='chgrp system $(DESTDIR)$(libexecdir)/libgtop_server && chmod g+s $(DESTDIR)$(libexecdir)/libgtop_server2'
	  ;;
	darwin*)
	  libgtop_sysdeps_dir=darwin
	  libgtop_need_server=yes
	  libgtop_have_sysinfo=yes
	  libgtop_postinstall='chgrp kmem $(DESTDIR)$(libexecdir)/libgtop_server2 && chmod g+s $(DESTDIR)$(libexecdir)/libgtop_server2'
	  ;;
	cygwin*)
	  libgtop_sysdeps_dir=cygwin
	  libgtop_need_server=no
	  libgtop_have_sysinfo=yes
	  libgtop_sysdeps_private_mountlist=yes
	  ;;
	*)
	  if test x$hacker_mode = xyes ; then
	    case "$host_os" in
	    sunos4*)
	      #Please note that this port is obsolete and not working at
	      #all. It is only useful for people who want to fix it ... :-)
	      libgtop_sysdeps_dir=sun4
	      libgtop_need_server=yes
	      ;;
	    osf*)
	      libgtop_sysdeps_dir=osf1
	      libgtop_need_server=yes
	      ;;
	    *)
	      libgtop_sysdeps_dir=stub
	      libgtop_need_server=no
	      ;;
	    esac
	  else
	    libgtop_sysdeps_dir=stub
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
	  AC_CHECK_LIB(kvm, kvm_open, KVM_LIBS=-lkvm, KVM_LIBS=)
	  AC_SUBST(KVM_LIBS)

	  AC_CHECK_LIB(util, kinfo_getfile)
	  AC_CHECK_FUNCS(kinfo_getfile)

	  AC_CHECK_HEADERS(net/if_var.h,,, [
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>])
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

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__FreeBSD_kernel__)
#include <net/if_sppp.h>
#else
#include <i4b/sppp/if_sppp.h>
#endif
],[
	size_t size = sizeof (struct sppp);
], have_i4b=yes, have_i4b=no)
	  AC_MSG_RESULT($have_i4b)
	  if test x$have_i4b = xyes; then
	    AC_DEFINE(HAVE_I4B, 1, [Define if I4B is available])
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

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__FreeBSD_kernel__)
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
	      AC_DEFINE(HAVE_I4B_ACCT, 1,
                        [Define if I4B accounting is supported])
	    else
	      AC_WARN([
*** I4B accounting disabled - you won't get any PPP statistics.
*** Read "misc/i4b_acct.txt" in the LibGTop source directory
*** to see how to enable it.])
	    fi
	  fi

	  AC_MSG_CHECKING(what we need to define to get struct msginfo)
	  AC_CACHE_VAL(msginfo_cv_needs,
	    msginfo_cv_needs=
	    for def in nothing KERNEL _KERNEL; do
	      AC_COMPILE_IFELSE([AC_LANG_SOURCE([#define $def
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

int
main (void)
{
  struct msginfo mi;
  mi.msgmax = 0;
  return 0;
}])],
	        [
	          msginfo_cv_needs=$def
	          if test ${msginfo_cv_needs} = KERNEL; then
	            AC_DEFINE(STRUCT_MSGINFO_NEEDS_KERNEL, 1,
	              [Define to 1 if we need to define KERNEL to get 'struct msginfo'])
	          elif test ${msginfo_cv_needs} = _KERNEL; then
	            AC_DEFINE(STRUCT_MSGINFO_NEEDS__KERNEL, 1,
	              [Define to 1 if we need to define _KERNEL to get 'struct msginfo'])
	          fi
	        ]
	      )
	      test -n "${msginfo_cv_needs}" && break
	    done
	  )
	  AC_MSG_RESULT($msginfo_cv_needs)
	  if test -z "${msginfo_cv_needs}"; then
	    AC_MSG_ERROR([Could not find the definition of 'struct msginfo'])
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
	  AC_DEFINE_UNQUOTED(GLIBTOP_LINUX_VERSION_CODE, $os_version_code,
              [Same as LINUX_VERSION_CODE either from <linux/version.h> or from the running kernel (if there are no configured kernel sources).])
	  AC_MSG_RESULT($os_version_code)
	  ;;
	solaris*)
	  os_major_version=`uname -r | sed 's/\([[0-9]]*\).\([[0-9]]*\)\.*\([[0-9]]*\)/\1/'`
	  os_minor_version=`uname -r | sed 's/\([[0-9]]*\).\([[0-9]]*\)\.*\([[0-9]]*\)/\2/'`
	  os_micro_version=`uname -r | sed 's/\([[0-9]]*\).\([[0-9]]*\)\.*\([[0-9]]*\)/\3/'`
	  test -z "$os_micro_version" && os_micro_version=0
	  os_version_expr="$os_major_version 10000 * $os_minor_version 100 * + $os_micro_version + p q"
	  os_version_code=`echo "$os_version_expr" | dc`

	  AC_MSG_CHECKING(for Solaris release code)
	  AC_DEFINE_UNQUOTED(GLIBTOP_SOLARIS_RELEASE, $os_version_code,
              [Solaris release code (eg. 20501 for Solaris 2.5.1).])
	  AC_MSG_RESULT($os_version_code)
	  ;;
	esac

	AC_MSG_CHECKING(whether we need libgtop)
	AC_MSG_RESULT($libgtop_need_server)

	if test x$libgtop_need_server = xyes ; then
	  AC_DEFINE(LIBGTOP_NEED_SERVER, 1, [Define if libgtop server is required])
	fi

	AM_CONDITIONAL(LIBGTOP_NEED_SERVER, test x$libgtop_need_server = xyes)
	AM_CONDITIONAL(LIBGTOP_SYSDEPS_PRIVATE_MOUNTLIST, test x$libgtop_sysdeps_private_mountlist = xyes)
	AM_CONDITIONAL(LIBGTOP_SYSDEPS_PRIVATE_FSUSAGE, test x$libgtop_sysdeps_private_fsusage = xyes)
])

