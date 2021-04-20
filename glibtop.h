/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __GLIBTOP_H__
#define __GLIBTOP_H__

/* for pid_t, uid_t, gid_t */
#include <sys/types.h>


#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _glibtop		glibtop;
typedef struct _glibtop_machine	glibtop_machine;

/**
 *  LIBGTOP_CHECK_VERSION
 *  @major: Major version number
 *  @minor: Minor version number
 *  @micro: Micro version number
 *
 *  Checks if the version given is compatable with this version of the
 *  library.  For example, LIBGTOP_CHECK_VERSION(1.2.3) would return TRUE
 *  if the version is 1.2.5, and FALSE if 1.1.0.  This can be used in
 *  build tests.
 *
 **/

#include <libgtopconfig.h>

#define LIBGTOP_CHECK_VERSION(major, minor, micro)    \
(LIBGTOP_MAJOR_VERSION > (major) || \
(LIBGTOP_MAJOR_VERSION == (major) && LIBGTOP_MINOR_VERSION > (minor)) || \
(LIBGTOP_MAJOR_VERSION == (major) && LIBGTOP_MINOR_VERSION == (minor) && \
LIBGTOP_MICRO_VERSION >= (micro)))


#include <glibtop/global.h>

#include <glibtop_server.h>

#ifndef GLIBTOP_MOUNTENTRY_LEN
#define GLIBTOP_MOUNTENTRY_LEN	79
#endif


#include <glibtop/sysdeps.h>

#include <glibtop/close.h>


struct _glibtop
{
	unsigned flags;
	unsigned method;		/* Server Method */
	unsigned error_method;		/* Error Method */
	int input [2];			/* Pipe client <- server */
	int output [2];			/* Pipe client -> server */
	int socket;			/* Accepted connection of a socket */
	int ncpu;			/* Number of CPUs, zero if single-processor */
	int real_ncpu;			/* Real number of CPUs. Only ncpu are monitored */
	unsigned long os_version_code;	/* Version code of the operating system */
	const char *name;		/* Program name for error messages */
	const char *server_command;	/* Command used to invoke server */
	const char *server_host;	/* Host the server should run on */
	const char *server_user;	/* Name of the user on the target host */
	const char *server_rsh;		/* Command used to connect to the target host */
	unsigned long features;		/* Server is required for this features */
	unsigned long server_port;	/* Port on which daemon is listening */
	glibtop_sysdeps sysdeps;	/* Detailed feature list */
	glibtop_sysdeps required;	/* Required feature list */
	pid_t pid;			/* PID of the server */

	uid_t uid;
	uid_t euid;
	gid_t gid;
	gid_t egid;

	glibtop_machine *machine;	/* Machine dependent data */

	int ndisk;			/* Number of DISKs,  zero if single-disk. This pertains to disk volumes */
	int real_ndisk;			/* Number of PHYSICAL DISKs. Only ndisk is monitored for now */
};

extern glibtop *glibtop_global_server;

extern const unsigned long glibtop_server_features;

/**
 * glibtop_init:
 *
 * Returns: (transfer none):
 */
glibtop *
glibtop_init (void);


/**
 * glibtop_init_r:
 * @server_ptr: (out):
 * @features:
 * @flags:
 *
 * Returns: (transfer none):
 */
glibtop *
glibtop_init_r (glibtop **server_ptr,
		unsigned long features,
		unsigned flags);


/**
 * glibtop_init_s: (skip)
 * @server_ptr: (out):
 * @features:
 * @flags:
 *
 * Returns: (transfer none):
 */
glibtop *
glibtop_init_s (glibtop **server_ptr,
		unsigned long features,
		unsigned flags);

GType     glibtop_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif
