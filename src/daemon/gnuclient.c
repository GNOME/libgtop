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

#include "gnuserv.h"
#include "getopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <signal.h>

#if !defined(SYSV_IPC) && !defined(UNIX_DOMAIN_SOCKETS) && \
    !defined(INTERNET_DOMAIN_SOCKETS)
int
main (int argc, char *argv[])
{
  fprintf (stderr, "Sorry, the Emacs server is only "
	   "supported on systems that have\n");
  fprintf (stderr, "Unix Domain sockets, Internet Domain "
	   "sockets or System V IPC.\n");
  exit (1);
} /* main */
#else /* SYSV_IPC || UNIX_DOMAIN_SOCKETS || INTERNET_DOMAIN_SOCKETS */

static char cwd[MAXPATHLEN+2];	/* current working directory when calculated */
static char *cp = NULL;		/* ptr into valid bit of cwd above */

static pid_t emacs_pid;			/* Process id for emacs process */

void initialize_signals (void);

static void
tell_emacs_to_resume (int sig)
{
  char buffer[GSERV_BUFSZ+1];
  int s;			/* socket / msqid to server */
  int connect_type;		/* CONN_UNIX, CONN_INTERNET, or
				   ONN_IPC */

  /* Why is SYSV so retarded? */
  /* We want emacs to realize that we are resuming */
  signal(SIGCONT, tell_emacs_to_resume); 

  connect_type = make_connection (NULL, (u_short) 0, &s);

  sprintf(buffer,"(gnuserv-eval '(resume-pid-console %d))", (int)getpid());
  send_string(s, buffer);

#ifdef SYSV_IPC
  if (connect_type == (int) CONN_IPC)
    disconnect_from_ipc_server (s, msgp, FALSE);
#else /* !SYSV_IPC */
  if (connect_type != (int) CONN_IPC)
    disconnect_from_server (s, FALSE);
#endif /* !SYSV_IPC */
}

static void
pass_signal_to_emacs (int sig)
{
  if (kill (emacs_pid, sig) == -1)
    {
      fprintf (stderr, "gnuattach: Could not pass signal to emacs process\n");
      exit (1);
    }
  initialize_signals ();
}

void
initialize_signals ()
{
  /* Set up signal handler to pass relevant signals to emacs process.
     We used to send SIGSEGV, SIGBUS, SIGPIPE, SIGILL and others to
     Emacs, but I think it's better not to.  I can see no reason why
     Emacs should SIGSEGV whenever gnuclient SIGSEGV-s, etc.  */
  signal (SIGHUP, pass_signal_to_emacs);
  signal (SIGQUIT, pass_signal_to_emacs);
  signal (SIGINT, pass_signal_to_emacs);
#ifdef SIGWINCH
  signal (SIGWINCH, pass_signal_to_emacs);
#endif

  /* We want emacs to realize that we are resuming */
  signal (SIGCONT, tell_emacs_to_resume);
}


/*
  get_current_working_directory -- return the cwd.
*/
static char *
get_current_working_directory (void)
{
  if (cp == NULL)
    {				/* haven't calculated it yet */
#ifdef BSD
      if (getwd (cwd) == 0)
#else /* !BSD */
      if (getcwd (cwd,MAXPATHLEN) == NULL)
#endif /* !BSD */
	{
	  perror (progname);
	  fprintf (stderr, "%s: unable to get current working directory\n",
		   progname);
	  exit (1);
	} /* if */
    
      /* on some systems, cwd can look like '@machine/' ... */
      /* ignore everything before the first '/' */
      for (cp = cwd; *cp && *cp != '/'; ++cp)
	;

    } /* if */

  return cp;

} /* get_current_working_directory */


/*
  filename_expand -- try to convert the given filename into a fully-qualified
  		     pathname.
*/
static void
filename_expand (char *fullpath, char *filename)
  /* fullpath - returned full pathname */
  /* filename - filename to expand */
{
  int len;

  fullpath[0] = '\0';
  
  if (filename[0] && filename[0] != '/')
    {	/* relative filename */
      strcat (fullpath, get_current_working_directory ());
      len = strlen (fullpath);

      if (len > 0 && fullpath[len-1] == '/')	/* trailing slash already? */
	;					/* yep */
      else
	strcat (fullpath, "/");		/* nope, append trailing slash */
    } /* if */

  strcat (fullpath,filename);

} /* filename_expand */

/* Encase the string in quotes, escape all the backslashes and quotes
   in string.  */
static char *
clean_string (CONST char *s)
{
  int i = 0;
  char *p, *res;

  for (p = s; *p; p++, i++)
    {
      if (*p == '\\' || *p == '\"')
	++i;
      else if (*p == '\004')
	i += 3;
    }
  p = res = (char *)malloc (i + 2 + 1);
  *p++ = '\"';
  for (; *s; p++, s++)
    {
      switch (*s)
	{
	case '\\':
	  *p++ = '\\';
	  *p = '\\';
	  break;
	case '\"':
	  *p++ = '\\';
	  *p = '\"';
	  break;
	case '\004':
	  *p++ = '\\';
	  *p++ = 'C';
	  *p++ = '-';
	  *p = 'd';
	  break;
	default:
	  *p = *s;
	}
    }
  *p++ = '\"';
  *p = '\0';
  return res;
}

#define GET_ARGUMENT(var, desc) do {					   \
 if (*(p + 1)) (var) = p + 1;						   \
   else									   \
     {									   \
       if (!argv[++i])							   \
         {								   \
           fprintf (stderr, "%s: `%s' must be followed by an argument\n",  \
		    progname, desc);					   \
	   exit (1);							   \
         }								   \
      (var) = argv[i];							   \
    }									   \
  over = 1;								   \
} while (0)

/* A strdup immitation. */
static char *
my_strdup (CONST char *s)
{
  char *new = malloc (strlen (s) + 1);
  if (new)
    strcpy (new, s);
  return new;
}

int
main (int argc, char *argv[])
{
  int starting_line = 1;	/* line to start editing at */
  char command[MAXPATHLEN+50];	/* emacs command buffer */
  char fullpath[MAXPATHLEN+1];	/* full pathname to file */
  char *eval_form = NULL;	/* form to evaluate with `-eval' */
  char *eval_function = NULL;	/* function to evaluate with `-f' */
  char *load_library = NULL;	/* library to load */
  int quick = 0;	       	/* quick edit, don't wait for user to
				   finish */
  int batch = 0;		/* batch mode */
  int view = 0;			/* view only. */
  int nofiles = 0;
  int errflg = 0;		/* option error */
  int s;			/* socket / msqid to server */
  int connect_type;		/* CONN_UNIX, CONN_INTERNET, or
				 * CONN_IPC */
  int suppress_windows_system = 0;
  char *display = NULL;
#ifdef INTERNET_DOMAIN_SOCKETS
  char *hostarg = NULL;		/* remote hostname */
  char *remotearg;
  char thishost[HOSTNAMSZ];	/* this hostname */
  char remotepath[MAXPATHLEN+1]; /* remote pathname */
  char *path;
  int rflg = 0;			/* pathname given on cmdline */
  char *portarg;
  u_short port = 0;		/* port to server */
#endif /* INTERNET_DOMAIN_SOCKETS */
#ifdef SYSV_IPC
  struct msgbuf *msgp;		/* message */
#endif /* SYSV_IPC */
  char *tty = NULL;
  char buffer[GSERV_BUFSZ + 1];	/* buffer to read pid */
  char result[GSERV_BUFSZ + 1];
  int i;

#ifdef INTERNET_DOMAIN_SOCKETS
  memset (remotepath, 0, sizeof (remotepath));
#endif /* INTERNET_DOMAIN_SOCKETS */

  progname = strrchr (argv[0], '/');
  if (progname)
    ++progname;
  else
    progname = argv[0];

  display = getenv ("DISPLAY");
  if (!display)
    suppress_windows_system = 1;
  else
    display = my_strdup (display);

  for (i = 1; argv[i] && !errflg; i++)
    {
      if (*argv[i] != '-')
	break;
      else if (*argv[i] == '-'
	       && (*(argv[i] + 1) == '\0'
		   || (*(argv[i] + 1) == '-' && *(argv[i] + 2) == '\0')))
	{
	  /* `-' or `--' */
	  ++i;
	  break;
	}

      if (!strcmp (argv[i], "-batch") || !strcmp (argv[i], "--batch"))
	batch = 1;
      else if (!strcmp (argv[i], "-eval") || !strcmp (argv[i], "--eval"))
	{
	  if (!argv[++i])
	    {
	      fprintf (stderr, "%s: `-eval' must be followed by an argument\n",
		       progname);
	      exit (1);
	    }
	  eval_form = argv[i];
	}
      else if (!strcmp (argv[i], "-display") || !strcmp (argv[i], "--display"))
	{
	  suppress_windows_system = 0;
	  if (!argv[++i])
	    {
	      fprintf (stderr,
		       "%s: `-display' must be followed by an argument\n",
		       progname);
	      exit (1);
	    }
	  if (display)
	    free (display);
	  /* no need to strdup. */
	  display = argv[i];
	}
      else if (!strcmp (argv[i], "-nw"))
	suppress_windows_system = 1;
      else
	{
	  /* Iterate over one-letter options. */
	  char *p;
	  int over = 0;
	  for (p = argv[i] + 1; *p && !over; p++)
	    {
	      switch (*p)
		{
		case 'q':
		  quick = 1;
		  break;
		case 'v':
		  view = 1;
		  break;
		case 'f':
		  GET_ARGUMENT (eval_function, "-f");
		  break;
		case 'l':
		  GET_ARGUMENT (load_library, "-l");
		  break;
#ifdef INTERNET_DOMAIN_SOCKETS
		case 'h':
		  GET_ARGUMENT (hostarg, "-h");
		  break;
		case 'p':
		  GET_ARGUMENT (portarg, "-p");
		  port = atoi (portarg);
		  break;
		case 'r':
		  GET_ARGUMENT (remotearg, "-r");
		  strcpy (remotepath, remotearg);
		  rflg = 1;
		  break;
#endif /* INTERNET_DOMAIN_SOCKETS */
		default:
		  errflg = 1;
		}
	    } /* for */
	} /* else */
    } /* for */

  if (errflg)
    {
      fprintf (stderr,
#ifdef INTERNET_DOMAIN_SOCKETS
	       "usage: %s [-nw] [-display display] [-q] [-v] [-l library]\n"
               "       [-batch] [-f function] [-eval form]\n"
	       "       [-h host] [-p port] [-r remote-path] [[+line] file] ...\n",
#else /* !INTERNET_DOMAIN_SOCKETS */
	       "usage: %s [-nw] [-q] [-v] [-l library] [-f function] [-eval form] "
	       "[[+line] path] ...\n",
#endif /* !INTERNET_DOMAIN_SOCKETS */
	       progname);
      exit (1);
    }
  if (batch && argv[i])
    {
      fprintf (stderr, "%s: Cannot specify `-batch' with file names\n",
	       progname);
      exit (1);
    }
  if (suppress_windows_system && hostarg)
    {
      fprintf (stderr, "%s: Remote editing is available only on X\n",
	       progname);
      exit (1);
    }

  *result = '\0';
  if (eval_function || eval_form || load_library)
    {
#if defined(INTERNET_DOMAIN_SOCKETS)
      connect_type = make_connection (hostarg, port, &s);
#else
      connect_type = make_connection (NULL, (u_short) 0, &s);
#endif
      sprintf (command, "(gnuserv-eval%s '(progn ", quick ? "-quickly" : "");
      send_string (s, command);
      if (load_library)
	{
	  sprintf (command, " (load-library %s)", clean_string (load_library));
	  send_string (s, command);
	}
      if (eval_form)
	{
	  sprintf (command, " %s", eval_form);
	  send_string (s, command);
	}
      if (eval_function)
	{
	  sprintf (command, " (%s)", eval_function);
	  send_string (s, command);
	}
      send_string (s, "))");
      send_string (s, EOT_STR);
      if (read_line (s, result) == 0)
	{
	  fprintf (stderr, "%s: Could not read\n", progname);
	  exit (1);
	}
    } /* eval_function || eval_form || load_library */
  else if (batch)
    {
      fprintf (stderr, "%s: `-batch' requires an evaluation\n",
	       progname);
      exit (1);
    }

  if (!batch)
    {
      if (suppress_windows_system)
	{
	  tty = ttyname (0);
	  if (!tty)
	    {
	      fprintf (stderr, "%s: Not connected to a tty", progname);
	      exit (1);
	    }
#if defined(INTERNET_DOMAIN_SOCKETS)
	  connect_type = make_connection (hostarg, port, &s);
#else
	  connect_type = make_connection (NULL, (u_short) 0, &s);
#endif
	  send_string (s, "(gnuserv-eval '(emacs-pid))");
	  send_string (s, EOT_STR);

	  if (read_line (s, buffer) == 0)
	    {
	      fprintf (stderr, "%s: Could not establish Emacs procces id\n",
		       progname);
	      exit (1);
	    }
      /* Don't do disconnect_from_server becasue we have already read
	 data, and disconnect doesn't do anything else. */
#ifndef INTERNET_DOMAIN_SOCKETS
	  if (connect_type == (int) CONN_IPC)
	    disconnect_from_ipc_server (s, msgp, FALSE);
#endif /* !SYSV_IPC */

	  emacs_pid = (pid_t)atol(buffer);
	  initialize_signals();
	} /* suppress_windows_system */

#if defined(INTERNET_DOMAIN_SOCKETS)
      connect_type = make_connection (hostarg, port, &s);
#else
      connect_type = make_connection (NULL, (u_short) 0, &s);
#endif

#ifdef INTERNET_DOMAIN_SOCKETS
      if (connect_type == (int) CONN_INTERNET)
	{
	  char *ptr;
	  gethostname (thishost, HOSTNAMSZ);
	  if (!rflg)
	    {				/* attempt to generate a path 
					 * to this machine */
	      if ((ptr = getenv ("GNU_NODE")) != NULL)
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
	{
	  fprintf (stderr, "%s: not enough memory for message buffer\n", progname);
	  exit (1);
	} /* if */

      msgp->mtext[0] = '\0';			/* ready for later strcats */
#endif /* SYSV_IPC */

      if (suppress_windows_system)
	{
	  char *term = getenv ("TERM");
	  if (!term)
	    {
	      fprintf (stderr, "%s: unknown terminal type\n", progname);
	      exit (1);
	    }
	  sprintf (command, "(gnuserv-edit-files '(tty %s %s %d) '(",
		   clean_string (tty), clean_string (term), (int)getpid ());
	}
      else /* !suppress_windows_system */
	{
	  sprintf (command, "(gnuserv-edit-files '(x %s) '(",
		   clean_string (display));
	} /* !suppress_windows_system */
      send_string (s, command);

      if (!argv[i])
	nofiles = 1;

      for (; argv[i]; i++)
	{
	  if (i < argc - 1 && *argv[i] == '+')
	    starting_line = atoi (argv[i++]);
	  else
	    starting_line = 1;
	  /* If the last argument is +something, treat it as a file. */
	  if (i == argc)
	    {
	      starting_line = 1;
	      --i;
	    }
	  filename_expand (fullpath, argv[i]);
#ifdef INTERNET_DOMAIN_SOCKETS
	  path = malloc (strlen (remotepath) + strlen (fullpath) + 1);
	  sprintf (path, "%s%s", remotepath, fullpath);
#else
	  path = my_strdup (fullpath);
#endif
	  sprintf (command, "(%d . %s)", starting_line, clean_string (path));
	  send_string (s, command);
	  free (path);
	} /* for */

      sprintf (command, ")%s%s",
	       (quick || (nofiles && !suppress_windows_system)) ? " 'quick" : "",
	       view ? " 'view" : "");
      send_string (s, command);
      send_string (s, ")");

#ifdef SYSV_IPC
      if (connect_type == (int) CONN_IPC)
	disconnect_from_ipc_server (s, msgp, FALSE);
#else /* !SYSV_IPC */
      if (connect_type != (int) CONN_IPC)
	disconnect_from_server (s, FALSE);
#endif /* !SYSV_IPC */
    } /* not batch */

  if (batch && !quick)
      printf ("%s\n", result);

  return 0;

} /* main */

#endif /* SYSV_IPC || UNIX_DOMAIN_SOCKETS || INTERNET_DOMAIN_SOCKETS */
