/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef __GLIBTOP_GLOBAL_H__
#define __GLIBTOP_GLOBAL_H__

#include <config.h>

/* __BEGIN_DECLS should be used at the beginning of your declarations,
   so that C++ compilers don't mangle their names.  Use __END_DECLS at
   the end of C declarations. */
   
#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

/* __P is a macro used to wrap function prototypes, so that compilers
   that don't understand ANSI C prototypes still work, and ANSI C
   compilers can issue warnings about type mismatches. */

#undef __P
#if defined (__STDC__) || defined (_AIX) \
|| (defined (__mips) && defined (_SYSTYPE_SVR4)) \
     || defined(WIN32) || defined(__cplusplus)
# define __P(protos) protos
#else
# define __P(protos) ()
#endif

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
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

__BEGIN_DECLS

#ifndef _
#define _(String) gettext (String)
#define N_(String) (String)
#endif

#ifndef HAVE_STRERROR
extern char *strerror __P((int));
#endif

__END_DECLS

#endif
