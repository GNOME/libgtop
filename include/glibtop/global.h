/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
   License for more details.

   You should have received a copy of the GNU Library General Public
   License along with LibGTop; see the file COPYING.LIB.  If not, write
   to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __GLIBTOP_GLOBAL_H__
#define __GLIBTOP_GLOBAL_H__

#ifdef _IN_LIBGTOP
#include <config.h>
#endif

/*
 * All declarations are enclosed in BEGIN_LIBGTOP_DECLS and
 * END_LIBGTOP_DECLS so that C++ compilers don't mangle their names.
 *
 */
   
#undef BEGIN_LIBGTOP_DECLS
#undef END_LIBGTOP_DECLS
#ifdef __cplusplus
# define BEGIN_LIBGTOP_DECLS extern "C" {
# define END_LIBGTOP_DECLS }
#else
# define BEGIN_LIBGTOP_DECLS /* empty */
# define END_LIBGTOP_DECLS /* empty */
#endif

#ifdef _IN_LIBGTOP

/* Provide macros to feature the GCC function attribute.
 */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define G_GNUC_NORETURN				\
  __attribute__((noreturn))
#define G_GNUC_CONST				\
  __attribute__((const))
#define G_GNUC_UNUSED				\
  __attribute__((unused))
#else	/* !__GNUC__ */
#define G_GNUC_NORETURN
#define G_GNUC_CONST
#define	G_GNUC_UNUSED
#endif	/* !__GNUC__ */

#endif /* _IN_LIBGTOP */

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef NEED_GNOMESUPPORT_H
#include <gnomesupport.h>
#endif

#ifdef WITHOUT_GUILE
#undef GLIBTOP_GUILE
#undef GLIBTOP_GUILE_NAMES
#endif

#ifdef GLIBTOP_GUILE
#include <guile/gh.h>
#endif

#ifdef _IN_LIBGTOP
#if (defined HAVE_LIBINTL_H) || (defined HAVE_GETTEXT)
#include <libintl.h>
#else
#include <libgettext.h>
#endif
#endif

#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#include <string.h>

#ifdef _IN_LIBGTOP

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>

#endif /* _IN_LIBGTOP */

#include <signal.h>
#include <errno.h>

#ifdef _IN_LIBGTOP

BEGIN_LIBGTOP_DECLS

#ifndef _
#define _(String) dgettext (PACKAGE, String)
#define N_(String) (String)
#endif

#ifndef HAVE_STRERROR
char *strerror (int errno);
#endif

END_LIBGTOP_DECLS

#endif /* _IN_LIBGTOP */

#endif
