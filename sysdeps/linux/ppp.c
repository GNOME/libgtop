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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/ppp.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <ctype.h>

#include <glib.h>

#ifdef HAVE_ISDN_H
#include <linux/isdn.h>
#else
#define ISDN_MAX_CHANNELS 64
#define IIOCGETCPS  _IO('I',21)
#endif /* HAVE_ISDN_H */

static const unsigned long _glibtop_sysdeps_ppp =
(1L << GLIBTOP_PPP_STATE) + (1L << GLIBTOP_PPP_BYTES_IN) +
(1L << GLIBTOP_PPP_BYTES_OUT);

/* Init function. */

void
_glibtop_init_ppp_s (glibtop *server)
{
	server->sysdeps.ppp = _glibtop_sysdeps_ppp;
}

static gboolean
get_ISDN_stats (glibtop *server, int *in, int *out)
{
	unsigned long isdn_stats[2 * ISDN_MAX_CHANNELS], *ptr;
	int fd;

	*in = *out = 0;

	fd = open ("/dev/isdninfo", O_RDONLY);
	if (fd < 0) {
		return FALSE;
	}

	if ((ioctl (fd, IIOCGETCPS, isdn_stats) < 0) && (errno != 0)) {
		close(fd);
		return FALSE;
	}

	for (ptr = isdn_stats;
	     ptr != (isdn_stats + G_N_ELEMENTS(isdn_stats));
	     /* NOOP */) {
		*in  += *ptr++; *out += *ptr++;
	}

	close (fd);
	return TRUE;
}

static gboolean is_ISDN_on (glibtop *server, int *online)
{
	FILE *f = NULL;
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

void
glibtop_get_ppp_s (glibtop *server, glibtop_ppp *buf, unsigned short device)
{
	int in, out, online;

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
