/*
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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

#ifndef __UTILS_H__
#define __UTILS_H__

G_BEGIN_DECLS

extern off_t
_glibtop_get_kmem_offset(glibtop* server, char* kname);

extern int
_glibtop_get_kmem_info(glibtop* server, off_t offset, void* buf, size_t len);

extern struct procsinfo*
_glibtop_get_procinfo (glibtop *server, pid_t pid);

#ifndef HAVE_VMGETINFO
extern  int
_glibtop_vmgetinfo (void *out, int command, int arg);
#endif

G_END_DECLS

#endif /* __UTILS_H__ */
