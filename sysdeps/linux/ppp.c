/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, October 1998.

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
#include <glibtop/xmalloc.h>
#include <glibtop/ppp.h>

#include <linux/isdn.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <ctype.h>

#include <glib.h>

static const unsigned long _glibtop_sysdeps_ppp =
(1L << GLIBTOP_PPP_STATE) + (1L << GLIBTOP_PPP_BYTES_IN) +
(1L << GLIBTOP_PPP_BYTES_OUT);

/* Init function. */

int
glibtop_init_ppp_s (glibtop *server)
{
	server->sysdeps.ppp = _glibtop_sysdeps_ppp;
}

static int
get_ISDN_stats (glibtop *server, int *in, int *out)
{
	unsigned long *isdn_stats, *ptr;
	int fd, i;
	
	*in = *out = 0;

	isdn_stats = glibtop_calloc_r (server, ISDN_MAX_CHANNELS * 2,
				       sizeof (unsigned long));

	fd = open ("/dev/isdninfo", O_RDONLY);
	if (fd < 0) {
		glibtop_free_r (server, isdn_stats);
		return FALSE;
	}

	if ((ioctl (fd, IIOCGETCPS, isdn_stats) < 0) && (errno != 0)) {
		glibtop_free_r (server, isdn_stats);
		close (fd);
		
		return FALSE;
	}

	for (i = 0, ptr = isdn_stats; i < ISDN_MAX_CHANNELS; i++) {
		*in  += *ptr++; *out += *ptr++;
	}

	glibtop_free_r (server, isdn_stats);
	close (fd);

	return TRUE;
}

static int is_ISDN_on (glibtop *server, int *online)
{
	FILE *f = 0;
	char buffer [BUFSIZ], *p;
	int i;

	/* Perhaps I should try to explain this code a little bit.
	 *
	 * ------------------------------------------------------------
	 * This is from the manpage of isdninfo(4):
	 *
	 * DESCRIPTION
	 *   /dev/isdninfo  is  a character device with major number 45
	 *   and minor number 255.  It delivers status information from
	 *   the Linux ISDN subsystem to user level.
	 *
	 * DATA FORMAT
	 *   When  reading  from this device, the current status of the
	 *   Linux ISDN subsystem is delivered in 6 lines of text. Each
	 *   line  starts  with  a  tag  string followed by a colon and
	 *   whitespace. After that the status values are appended sep-
	 *   arated by whitespace.
	 *
	 *   flags  is the tag of line 5. In this line for every driver
	 *          slot, it's B-Channel status is shown. If no  driver
	 *          is  registered  in a slot, a ? is shown.  For every
	 *          established B-Channel of the driver, a bit  is  set
	 *          in  the  shown value. The driver's first channel is
	 *          mapped to bit 0, the second channel to bit 1 and so
	 *          on.
	 * ------------------------------------------------------------
	 *
	 * So we open /dev/isdninfo, discard the first four lines of text
	 * and then check whether we have something that is not `0' or `?'
	 * in one of the flags fields.
	 *
	 * Sounds complicated, but I don't see any other way to check whether
	 * we are connected. Also, this is the method some other ISDN tools
	 * for Linux use.
	 *
	 * Martin
	 */

	f = fopen ("/dev/isdninfo", "r");

	if (!f) return FALSE;

	for (i = 0; i < 5; i++) {
		if (fgets (buffer, BUFSIZ, f) == NULL) {
			fclose (f);
			return FALSE;
		}
	}

	if (strncmp (buffer, "flags:", 6)) {
		fclose (f);
		return FALSE;
	}

	p = buffer+6;

	while (*p) {
		char *end = p;

		if (isspace (*p)) {
			p++;
			continue;
		}

		for (end = p; *end && !isspace (*end); end++)
			;

		if (*end == 0)
			break;
		else
			*end = 0;

		if (!strcmp (p, "?") || !strcmp (p, "0")) {
			p = end+1;
			continue;
		}

		fclose (f);

		*online = TRUE;
		return TRUE;
	}

	fclose (f);

	*online = FALSE;
	return TRUE;
}

/* Provides PPP/ISDN information. */

int
glibtop_get_ppp_s (glibtop *server, glibtop_ppp *buf, unsigned short device)
{
	int in, out, online;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PPP, 0);

	memset (buf, 0, sizeof (glibtop_ppp));

	if (is_ISDN_on (server, &online)) {
		buf->state = online ? GLIBTOP_PPP_STATE_ONLINE :
			GLIBTOP_PPP_STATE_HANGUP;
		buf->flags |= (1L << GLIBTOP_PPP_STATE);
	}

	if (get_ISDN_stats (server, &in, &out)) {
		buf->bytes_in = in;
		buf->bytes_out = out;
		buf->flags |= (1L << GLIBTOP_PPP_BYTES_IN) |
			(1L << GLIBTOP_PPP_BYTES_OUT);
	}
}
