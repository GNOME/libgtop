/* -*-C-*-
 Client code to allow local and remote editing of files by XEmacs.
 Copyright (C) 1989 Free Software Foundation, Inc.
 Copyright (C) 1995 Sun Microsystems, Inc.
 Copyright (C) 1997 Free Software Foundation, Inc.

This file is part of XEmacs.

XEmacs is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

XEmacs is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with XEmacs; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

 Author: Andy Norman (ange@hplb.hpl.hp.com), based on 
         'etc/emacsclient.c' from the GNU Emacs 18.52 distribution.

 Please mail bugs and suggestions to the author at the above address.
*/

/*
 * This file incorporates new features added by Bob Weiner <weiner@mot.com>,
 * Darrell Kindred <dkindred@cmu.edu> and Arup Mukherjee <arup@cmu.edu>.
 * GNUATTACH support added by Ben Wing <wing@xemacs.org>.
 * Please see the note at the end of the README file for details.
 *
 * (If gnuserv came bundled with your emacs, the README file is probably
 * ../etc/gnuserv.README relative to the directory containing this file)
 */

#if 0
/* Hand-munged RCS header */
static char rcsid [] = "!Header: gnuclient.c,v 2.2 95/12/12 01:39:21 wing nene !";
#endif

#include <glibtop/gnuserv.h>

#if !defined(SYSV_IPC) && !defined(UNIX_DOMAIN_SOCKETS) && !defined(INTERNET_DOMAIN_SOCKETS)
#else /* SYSV_IPC || UNIX_DOMAIN_SOCKETS || INTERNET_DOMAIN_SOCKETS */

int
main (int argc, char *argv[])
{
  int s;			/* socket / msqid to server */
  int connect_type;		/* CONN_UNIX, CONN_INTERNET, or CONN_IPC */
#ifdef INTERNET_DOMAIN_SOCKETS
  char thishost [HOSTNAMSZ];	/* this hostname */
#endif /* INTERNET_DOMAIN_SOCKETS */
#ifdef SYSV_IPC
  struct msgbuf *msgp;		/* message */
#endif /* SYSV_IPC */

  glibtop_init ();

  connect_type = glibtop_make_connection (NULL, (u_short) 0, &s);

#ifdef INTERNET_DOMAIN_SOCKETS
  if (connect_type == (int) CONN_INTERNET)
    {
      gethostname (thishost, HOSTNAMSZ);
    }
#endif /* INTERNET_DOMAIN_SOCKETS */

#ifdef SYSV_IPC
  if ((msgp = (struct msgbuf *) 
       malloc (sizeof *msgp + GSERV_BUFSZ)) == NULL)
    glibtop_error_io ("not enough memory for message buffer");
  
  msgp->mtext[0] = '\0';			/* ready for later strcats */
#endif /* SYSV_IPC */

  raise (SIGSTOP);

#ifdef SYSV_IPC
  if (connect_type == (int) CONN_IPC)
    disconnect_from_ipc_server (s, msgp, FALSE);
#else /* !SYSV_IPC */
  if (connect_type != (int) CONN_IPC)
    ; //    disconnect_from_server (s, FALSE);
#endif /* !SYSV_IPC */

  return 0;
} /* main */

#endif /* SYSV_IPC || UNIX_DOMAIN_SOCKETS || INTERNET_DOMAIN_SOCKETS */
