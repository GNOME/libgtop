/* $Id$ */

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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

#include <glibtop/xmalloc.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_swap =
(1 << GLIBTOP_SWAP_TOTAL) + (1 << GLIBTOP_SWAP_USED) +
(1 << GLIBTOP_SWAP_FREE) + (1 << GLIBTOP_SWAP_PAGEIN) +
(1 << GLIBTOP_SWAP_PAGEOUT);

#if defined(__FreeBSD__) || defined(__bsdi__)

#include <sys/conf.h>
#ifdef __bsdi__
#include <vm/swap_pager.h>
#else
#include <sys/rlist.h>
#endif
#include <sys/vmmeter.h>

/* nlist structure for kernel access */

#if defined(__bsdi__)
static struct nlist nlst [] = {
	{ "_swapstats" }, /* general swap info */
	{ 0 }
};
#elif __FreeBSD__ < 4
static struct nlist nlst [] = {
#define VM_SWAPLIST	0
	{ "_swaplist" },/* list of free swap areas */
#define VM_SWDEVT	1
	{ "_swdevt" },	/* list of swap devices and sizes */
#define VM_NSWAP	2
	{ "_nswap" },	/* size of largest swap device */
#define VM_NSWDEV	3
	{ "_nswdev" },	/* number of swap devices */
#define VM_DMMAX	4
	{ "_dmmax" },	/* maximum size of a swap block */
	{ 0 }
};
#endif

#elif defined(__NetBSD__)

#include <vm/vm_swap.h>

#endif

/* nlist structure for kernel access */
static struct nlist nlst2 [] = {
	{ "_cnt" },
	{ 0 }
};

/* Init function. */

void
glibtop_init_swap_p (glibtop *server)
{
#if defined(__FreeBSD__) || defined(__bsdi__)
#if __FreeBSD__ < 4 || defined(__bsdi__)
	if (kvm_nlist (server->machine.kd, nlst) != 0) {
		glibtop_warn_io_r (server, "kvm_nlist (swap)");
		return;
	}
#else
	struct kvm_swap dummy;

	if (kvm_getswapinfo (server->machine.kd, &dummy, 1, 0) != 0) {
		glibtop_warn_io_r (server, "kvm_swap (swap)");
		return;
	}
#endif
#endif

	if (kvm_nlist (server->machine.kd, nlst2) != 0) {
		glibtop_warn_io_r (server, "kvm_nlist (cnt)");
		return;
	}

	server->sysdeps.swap = _glibtop_sysdeps_swap;
}

/* Provides information about swap usage. */

/*
 * This function is based on a program called swapinfo written
 * by Kevin Lahey <kml@rokkaku.atl.ga.us>.
 */

void
glibtop_get_swap_p (glibtop *server, glibtop_swap *buf)
{
#if defined(__FreeBSD__)

# if __FreeBSD__ < 4
	char *header;
	int hlen, nswdev, dmmax;
	int div, nfree, npfree;
	struct swdevt *sw;
	long blocksize, *perdev;
	struct rlist head;
	struct rlisthdr swaplist;
	struct rlist *swapptr;
	size_t sw_size;
	u_long ptr;
# else
	int nswdev;
	struct kvm_swap kvmsw[16];
# endif

#elif defined(__bsdi__)	
	struct swapstats swap;
#elif defined(__NetBSD__)
	struct swapent *swaplist;
#endif

	int nswap, i;
	int avail = 0, inuse = 0;

	/* To get `pagein' and `pageout'. */
	struct vmmeter vmm;
        static int swappgsin = -1;
	static int swappgsout = -1;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_SWAP), 0);
	
	memset (buf, 0, sizeof (glibtop_swap));

	if (server->sysdeps.swap == 0)
		return;

	/* This is used to get the `pagein' and `pageout' members. */
	
	if (kvm_read (server->machine.kd, nlst2[0].n_value,
		      &vmm, sizeof (vmm)) != sizeof (vmm)) {
		glibtop_warn_io_r (server, "kvm_read (cnt)");
		return;
	}

        if (swappgsin < 0) {
		buf->pagein = 0;
		buf->pageout = 0;
	} else {
#ifdef __FreeBSD__
		buf->pagein = vmm.v_swappgsin - swappgsin;
		buf->pageout = vmm.v_swappgsout - swappgsout;
#else
		buf->pagein = vmm.v_swpin - swappgsin;
		buf->pageout = vmm.v_swpout - swappgsout;
#endif
	}

#ifdef __FreeBSD__
        swappgsin = vmm.v_swappgsin;
	swappgsout = vmm.v_swappgsout;
#else
	swappgsin = vmm.v_swpin;
	swappgsout = vmm.v_swpout;
#endif

#if defined(__FreeBSD__)

#if __FreeBSD__ < 4

	/* Size of largest swap device. */

	if (kvm_read (server->machine.kd, nlst[VM_NSWAP].n_value,
		      &nswap, sizeof (nswap)) != sizeof (nswap)) {
		glibtop_warn_io_r (server, "kvm_read (nswap)");
		return;
	}

	/* Number of swap devices. */

	if (kvm_read (server->machine.kd, nlst[VM_NSWDEV].n_value,
		      &nswdev, sizeof (nswdev)) != sizeof (nswdev)) {
		glibtop_warn_io_r (server, "kvm_read (nswdev)");
		return;
	}

	/* Maximum size of a swap block. */

	if (kvm_read (server->machine.kd, nlst[VM_DMMAX].n_value,
		      &dmmax, sizeof (dmmax)) != sizeof (dmmax)) {
		glibtop_warn_io_r (server, "kvm_read (dmmax)");
		return;
	}

	/* List of free swap areas. */

	if (kvm_read (server->machine.kd, nlst[VM_SWAPLIST].n_value,
		      &swaplist, sizeof (swaplist)) != sizeof (swaplist)) {
		glibtop_warn_io_r (server, "kvm_read (swaplist)");
		return;
	}

	/* Kernel offset of list of swap devices and sizes. */

	if (kvm_read (server->machine.kd, nlst[VM_SWDEVT].n_value,
		      &ptr, sizeof (ptr)) != sizeof (ptr)) {
		glibtop_warn_io_r (server, "kvm_read (swdevt)");
		return;
	}

	/* List of swap devices and sizes. */

	sw_size = nswdev * sizeof (*sw);
	sw = glibtop_malloc_r (server, sw_size);

	if (kvm_read (server->machine.kd, ptr, sw, sw_size) != (ssize_t)sw_size) {
		glibtop_warn_io_r (server, "kvm_read (*swdevt)");
		return;
	}

	perdev = glibtop_malloc (nswdev * sizeof (*perdev));

	/* Count up swap space. */

	nfree = 0;
	memset (perdev, 0, nswdev * sizeof(*perdev));

	swapptr = swaplist.rlh_list;

	while (swapptr) {
		int	top, bottom, next_block;

		if (kvm_read (server->machine.kd, (int) swapptr, &head,
			      sizeof (struct rlist)) != sizeof (struct rlist)) {
			glibtop_warn_io_r (server, "kvm_read (swapptr)");
			return;
		}

		top = head.rl_end;
		bottom = head.rl_start;

		nfree += top - bottom + 1;

		/*
		 * Swap space is split up among the configured disks.
		 *
		 * For interleaved swap devices, the first dmmax blocks
		 * of swap space some from the first disk, the next dmmax
		 * blocks from the next, and so on up to nswap blocks.
		 *
		 * The list of free space joins adjacent free blocks,
		 * ignoring device boundries.  If we want to keep track
		 * of this information per device, we'll just have to
		 * extract it ourselves.
		 */
		while (top / dmmax != bottom / dmmax) {
			next_block = ((bottom + dmmax) / dmmax);
			perdev[(bottom / dmmax) % nswdev] +=
				next_block * dmmax - bottom;
			bottom = next_block * dmmax;
		}
		perdev[(bottom / dmmax) % nswdev] +=
			top - bottom + 1;

		swapptr = head.rl_next;
	}

	header = getbsize (&hlen, &blocksize);

	div = blocksize / 512;
	avail = npfree = 0;
	for (i = 0; i < nswdev; i++) {
		int xsize, xfree;

		/*
		 * Don't report statistics for partitions which have not
		 * yet been activated via swapon(8).
		 */
		if (!(sw[i].sw_flags & SW_FREED))
			continue;

		/* The first dmmax is never allocated to avoid trashing of
		 * disklabels
		 */
		xsize = sw[i].sw_nblks - dmmax;
		xfree = perdev[i];
		inuse = xsize - xfree;
		npfree++;
		avail += xsize;
	}

	/*
	 * If only one partition has been set up via swapon(8), we don't
	 * need to bother with totals.
	 */
	inuse = avail - nfree;

	glibtop_free_r (server, sw);
	glibtop_free_r (server, perdev);

	buf->flags = _glibtop_sysdeps_swap;

	buf->used = inuse;
	buf->free = avail;

	buf->total = inuse + avail;

#else

	nswdev = kvm_getswapinfo(server->machine.kd, kvmsw, 16, 0);

	buf->flags = _glibtop_sysdeps_swap;

	buf->used = kvmsw[nswdev].ksw_used;
	buf->total = kvmsw[nswdev].ksw_total;

	buf->free = buf->total - buf->used;

#endif

#elif defined(__bsdi__)

	/* General info about swap devices. */

	if (kvm_read (server->machine.kd, nlst[0].n_value,
		      &swap, sizeof (swap)) != sizeof (swap)) {
		glibtop_warn_io_r (server, "kvm_read (swap)");
		return;
	}

	buf->flags = _glibtop_sysdeps_swap;

	buf->used = swap.swap_total - swap.swap_free;
	buf->free = swap.swap_free;

	buf->total = swap.swap_total;

#elif defined(__NetBSD__)

	nswap = swapctl (SWAP_NSWAP, NULL, 0);
	if (nswap < 0) {
		glibtop_warn_io_r (server, "swapctl (SWAP_NSWAP)");
		return;
	}

	swaplist = glibtop_calloc_r (server, nswap, sizeof (struct swapent));

	if (swapctl (SWAP_STATS, swaplist, nswap) != nswap) {
		glibtop_warn_io_r (server, "swapctl (SWAP_STATS)");
		glibtop_free_r (server, swaplist);
		return;
	}

	for (i = 0; i < nswap; i++) {
		avail += swaplist[i].se_nblks;
		inuse += swaplist[i].se_inuse;
	}

	glibtop_free_r (server, swaplist);

	buf->flags = _glibtop_sysdeps_swap;

	buf->used = inuse;
	buf->free = avail;

	buf->total = inuse + avail;
#endif
}
