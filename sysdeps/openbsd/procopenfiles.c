/* Copyright (C) 1998-99 Martin Baulig
   Copyright (C) 2004 Nicol\ufffds Lichtmaier
   Copyright (C) 2007 Joe Marcus Clarke
   Copyright (C) 2013 Robert Nagy
   This file is part of LibGTop 1.0.

   Modified by Nicol\ufffds Lichtmaier to give a process open files.

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

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procopenfiles.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#define _KERNEL
#include <sys/file.h>
#undef _KERNEL
#include <sys/mount.h>
#include <sys/un.h>
#include <sys/vnode.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <kvm.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_open_files =
(1L << GLIBTOP_PROC_OPEN_FILES_NUMBER)|
(1L << GLIBTOP_PROC_OPEN_FILES_TOTAL)|
(1L << GLIBTOP_PROC_OPEN_FILES_SIZE);

kvm_t *kd;

struct filearg {
	SLIST_ENTRY(filearg) next;
	dev_t dev;
	ino_t ino;
	char *name;
	TAILQ_HEAD(fuserhead, fuser) fusers;
};

SLIST_HEAD(fileargs, filearg);

struct fileargs fileargs = SLIST_HEAD_INITIALIZER(fileargs);

/* Init function. */

void
_glibtop_init_proc_open_files_s (glibtop *server)
{
	server->sysdeps.proc_open_files = _glibtop_sysdeps_proc_open_files;
}

static char *
addr_to_string(struct kinfo_file *kf)
{
	char *buffer = NULL;
	struct in_addr faddr;

	memcpy(&faddr, kf->inp_faddru, sizeof(faddr));

	if ((kf->so_protocol == IPPROTO_TCP) ||
	    (kf->so_protocol == IPPROTO_UDP)) {
		if (kf->inp_fport) {
			buffer = g_strdup(faddr.s_addr == INADDR_ANY ? "*" :
 			    inet_ntoa(faddr));
		}
	}

	return buffer;
}

glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf,	pid_t pid)
{
	struct kinfo_file *kf, *kflast;
	int cnt;
	char kbuf[_POSIX2_LINE_MAX];
	GArray *entries;

	memset(buf, 0, sizeof (glibtop_proc_open_files));

	if ((kd = kvm_openfiles(NULL, NULL, NULL, KVM_NO_FILES, kbuf)) == NULL)
		g_warning("%s", kbuf);

	if ((kf = kvm_getfiles(kd, KERN_FILE_BYPID, pid, sizeof(*kf), &cnt)) == NULL)
		g_warning("%s", kvm_geterr(kd));

	entries = g_array_new(FALSE, FALSE, sizeof(glibtop_open_files_entry));

	for (kflast = &kf[cnt]; kf < kflast; ++kf) {
		glibtop_open_files_entry entry = {0};

		if (kf->fd_fd < 0)
			continue;

		if (kf->f_type == DTYPE_SOCKET) {
			if (kf->so_family == AF_LOCAL) {
				entry.type = GLIBTOP_FILE_TYPE_LOCALSOCKET;
				static char *stypename[] = {
					"unused",	/* 0 */
					"stream",
					"dgram",
					"raw",
					"rdm",
					"seqpak"
				};
#define STYPEMAX 5
				char *stype, stypebuf[24];

				if (kf->so_type > STYPEMAX) {
					snprintf(stypebuf, sizeof(stypebuf), "?%d", kf->so_type);
					stype = stypebuf;
				} else {
					stype = stypename[kf->so_type];
				}
				g_strlcpy(entry.info.localsock.name, stype,
				    sizeof(entry.info.localsock.name));
			} else if (kf->so_family == AF_INET ||
				   kf->so_family == AF_INET6) {
				char *addrstr;

				if (kf->so_family == AF_INET)
					entry.type = GLIBTOP_FILE_TYPE_INETSOCKET;
				else
					entry.type = GLIBTOP_FILE_TYPE_INET6SOCKET;

				addrstr = addr_to_string(kf);
				g_strlcpy(entry.info.sock.dest_host,
					  addrstr,
					  sizeof(entry.info.sock.dest_host));
				g_free(addrstr);
				entry.info.sock.dest_port = kf->inp_fport ? ntohs(kf->inp_fport) : 0;
			}
		} else if (kf->f_type == DTYPE_PIPE) {
			entry.type = GLIBTOP_FILE_TYPE_PIPE;
		} else if (kf->f_type == DTYPE_VNODE) {
			int badtype = 0;
			char nbuf[MAXPATHLEN];

			entry.type = GLIBTOP_FILE_TYPE_FILE;

			if ((kf->v_type == VNON) ||
			    (kf->v_type == VBAD) ||
			    (kf->v_tag == VT_NON && !(kf->v_flag & VCLONE))) {
				badtype = 1;
			}

			if (badtype)
				continue;

			if ((kf->v_type == VBLK) ||
			    (kf->v_type == VCHR)) {
				snprintf(nbuf, sizeof(nbuf), "/dev/%s",
				    devname(kf->va_rdev, kf->v_type == VCHR ?  S_IFCHR : S_IFBLK));
			} else {
				snprintf(nbuf, sizeof(nbuf), "inode %llu on %s", kf->va_fileid,
				    kf->f_mntonname);
			}
			g_strlcpy(entry.info.file.name, nbuf,
			    sizeof(entry.info.file.name));
		} else
			continue;

		entry.fd = kf->fd_fd;

		g_array_append_val(entries, entry);
	}

	buf->flags = _glibtop_sysdeps_proc_open_files;
	buf->number = entries->len;
	buf->size = sizeof(glibtop_open_files_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_open_files_entry*)g_array_free(entries, FALSE);
}
