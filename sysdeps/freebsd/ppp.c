/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, October 1998.

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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/ppp.h>

#include <glibtop_suid.h>

#ifdef HAVE_I4B

#include <net/if.h>
#include <net/if_types.h>
#include <net/netisr.h>
#include <net/route.h>

#ifdef __FreeBSD__
#include <net/if_sppp.h>
#else
#include <i4b/sppp/if_sppp.h>
#endif

/* Read `misc/i4b_acct.txt' for details ... */
#ifdef HAVE_I4B_ACCT
#include <machine/i4b_acct.h>
#endif

static const unsigned long _glibtop_sysdeps_ppp =
(1 << GLIBTOP_PPP_STATE);

#ifdef HAVE_I4B_ACCT
static const unsigned long _glibtop_sysdeps_ppp_acct =
(1 << GLIBTOP_PPP_BYTES_IN) + (1 << GLIBTOP_PPP_BYTES_OUT);
#endif

#endif /* HAVE_I4B */

/* nlist structure for kernel access */
static struct nlist nlst [] = {
#ifdef HAVE_I4B
	{ "_i4bisppp_softc" },
#endif
	{ 0 }
};

/* Init function. */

void
glibtop_init_ppp_p (glibtop *server)
{
#ifdef HAVE_I4B
#ifdef HAVE_I4B_ACCT
	server->sysdeps.ppp = _glibtop_sysdeps_ppp |
		_glibtop_sysdeps_ppp_acct;
#else
	server->sysdeps.ppp = _glibtop_sysdeps_ppp;
#endif
#endif /* HAVE_I4B */

	if (kvm_nlist (server->machine.kd, nlst) != 0)
		glibtop_error_io_r (server, "kvm_nlist");
}

/* Provides information about ppp usage. */

void
glibtop_get_ppp_p (glibtop *server, glibtop_ppp *buf, unsigned short device)
{
#ifdef HAVE_I4B
#ifdef HAVE_I4B_ACCT
	struct i4bisppp_softc data;
#else
	struct sppp data;
#endif
	int phase;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PPP), 0);
	
	memset (buf, 0, sizeof (glibtop_ppp));

	if (kvm_read (server->machine.kd, nlst [0].n_value,
		      &data, sizeof (data)) != sizeof (data))
		glibtop_error_io_r (server, "kvm_read (i4bisppp_softc)");

#ifdef HAVE_I4B_ACCT
	phase = data.sc_if_un.scu_sp.pp_phase;
#else
	phase = data.pp_phase;
#endif

	switch (phase) {
	case PHASE_DEAD:
	case PHASE_TERMINATE:
		buf->state = GLIBTOP_PPP_STATE_HANGUP;
		break;
	case PHASE_ESTABLISH:
	case PHASE_NETWORK:
		buf->state = GLIBTOP_PPP_STATE_ONLINE;
		break;
	default:
		buf->state = GLIBTOP_PPP_STATE_UNKNOWN;
		break;
	}

	buf->flags = _glibtop_sysdeps_ppp;

#ifdef HAVE_I4B_ACCT
	buf->bytes_in = data.sc_iinb;
	buf->bytes_out = data.sc_ioutb;
	buf->flags |= _glibtop_sysdeps_ppp_acct;
#endif
#endif /* HAVE_I4B */
}
