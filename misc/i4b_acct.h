/* This file contains modified code from `/usr/src/i4b/driver/i4b_isppp.c'
 * which belongs to the I4B package.
 *
 * The original copyright of this file [/usr/src/i4b/driver/i4b_isppp.c] is
 *
 * -----------------------------------------------------------------------------
 *
 *   Copyright (c) 1997 Joerg Wunsch. All rights reserved.
 *
 *   Copyright (c) 1997, 1998 Hellmuth Michaelis. All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of the author nor the names of any co-contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *   4. Altered versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software and/or documentation.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 *   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *   SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------------
 */

#define I4BISPPPACCT	1	/* enable accounting messages */

struct i4bisppp_softc {
	/*
	 * struct sppp starts with a struct ifnet, but we gotta allocate
	 * more space for it.  NB: do not relocate this union, it must
	 * be first in isppp_softc.  The tls and tlf hooks below want to
	 * convert a ``struct sppp *'' into a ``struct isppp_softc *''.
	 */
	union {
		struct ifnet scu_if;
		struct sppp scu_sp;
	} sc_if_un;
#define sc_if sc_if_un.scu_if
	int	sc_state;	/* state of the interface	*/
#ifndef __FreeBSD__
	int	sc_unit;	/* unit number for Net/OpenBSD	*/
#endif
	void *sc_cdp;		/* ptr to call descriptor	*/

#ifdef I4BISPPPACCT
	int sc_iinb;		/* isdn driver # of inbytes	*/
	int sc_ioutb;		/* isdn driver # of outbytes	*/
	int sc_inb;		/* # of bytes rx'd		*/
	int sc_outb;		/* # of bytes tx'd	 	*/
	int sc_linb;		/* last # of bytes rx'd		*/
	int sc_loutb;		/* last # of bytes tx'd 	*/
	int sc_fn;		/* flag, first null acct	*/
#endif

#if defined(__FreeBSD__) && __FreeBSD__ >= 3
	struct callout_handle sc_ch;
#endif
};
