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
  int connect_type;		/* CONN_UNIX, CONN_INTERNET, or
				 * CONN_IPC */
#ifdef INTERNET_DOMAIN_SOCKETS
  char *hostarg = NULL;		/* remote hostname */
  char thishost[HOSTNAMSZ];	/* this hostname */
  char remotepath[MAXPATHLEN+1]; /* remote pathname */
  int rflg = 0;			/* pathname given on cmdline */
  u_short port = 0;		/* port to server */
#endif /* INTERNET_DOMAIN_SOCKETS */
#ifdef SYSV_IPC
  struct msgbuf *msgp;		/* message */
#endif /* SYSV_IPC */

  glibtop_init ();

#ifdef INTERNET_DOMAIN_SOCKETS
  memset (remotepath, 0, sizeof (remotepath));
#endif /* INTERNET_DOMAIN_SOCKETS */

#if defined(INTERNET_DOMAIN_SOCKETS)
  connect_type = glibtop_make_connection (hostarg, port, &s);
#else
  connect_type = glibtop_make_connection (NULL, (u_short) 0, &s);
#endif

#ifdef INTERNET_DOMAIN_SOCKETS
  if (connect_type == (int) CONN_INTERNET)
    {
      char *ptr;
      gethostname (thishost, HOSTNAMSZ);
      if (!rflg)
	{				/* attempt to generate a path 
					 * to this machine */
	  if ((ptr = getenv ("LIBGTOP_NODE")) != NULL)
	    /* user specified a path */
	    strcpy (remotepath, ptr);
	}
#if 0  /* This is really bogus... re-enable it if you must have it! */
#if defined (hp9000s300) || defined (hp9000s800)
      else if (strcmp (thishost,hostarg))
	{	/* try /net/thishost */
	  strcpy (remotepath, "/net/");		/* (this fails using internet 
						   addresses) */
	  strcat (remotepath, thishost);
	}
#endif
#endif
    }
  else
    {			/* same machines, no need for path */
      remotepath[0] = '\0';	/* default is the empty path */
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
