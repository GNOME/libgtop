/* -*-C-*-
 * Server code for handling requests from clients and forwarding them
 * on to the GNU Emacs process.
 * 
 * This file is part of GNU Emacs.
 * 
 * Copying is permitted under those conditions described by the GNU
 * General Public License.
 * 
 * Copyright (C) 1989 Free Software Foundation, Inc.
 * 
 * Author: Andy Norman (ange@hplb.hpl.hp.com), based on 'etc/server.c'
 * from the 18.52 GNU Emacs distribution.
 * 
 * Please mail bugs and suggestions to the author at the above address.
 */

/* HISTORY 
 * 11-Nov-1990                bristor@simba   
 *    Added EOT stuff.
 */

/*
 * This file incorporates new features added by Bob Weiner <weiner@mot.com>,
 * Darrell Kindred <dkindred@cmu.edu> and Arup Mukherjee <arup@cmu.edu>.
 * Please see the note at the end of the README file for details.
 *
 * (If gnuserv came bundled with your emacs, the README file is probably
 * ../etc/gnuserv.README relative to the directory containing this file)
 */

static char rcsid[] = "!Header: gnuserv.c,v 2.1 95/02/16 11:58:27 arup alpha !";

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>

#include "server_config.h"

#include <glibtop/gnuserv.h>

#include <fcntl.h>
#include <sys/wait.h>

#ifdef AIX
#include <sys/select.h>
#endif

extern void handle_parent_connection __P ((glibtop *, int));
extern void handle_child_connection __P ((glibtop *, int));

#if !defined(SYSV_IPC) && !defined(UNIX_DOMAIN_SOCKETS) && !defined(INTERNET_DOMAIN_SOCKETS)
main ()
{
	fprintf (stderr, "Sorry, the Emacs server is only supported on systems that have\n");
	fprintf (stderr, "Unix Domain sockets, Internet Domain sockets or System V IPC\n");
	exit (1);
}				/* main */

#else /* SYSV_IPC || UNIX_DOMAIN_SOCKETS || INTERNET_DOMAIN_SOCKETS */

#ifdef SYSV_IPC

int ipc_qid = 0;		/* ipc message queue id */
int ipc_wpid = 0;		/* watchdog task pid */


/*
 * ipc_exit -- clean up the queue id and queue, then kill the watchdog task
 * if it exists. exit with the given status.
 */
void
ipc_exit (int stat)
{
	msgctl (ipc_qid, IPC_RMID, 0);

	if (ipc_wpid != 0)
		kill (ipc_wpid, SIGKILL);

	exit (stat);
}				/* ipc_exit */


/*
 * ipc_handle_signal -- catch the signal given and clean up.
 */
void
ipc_handle_signal (int sig)
{
	ipc_exit (0);
}				/* ipc_handle_signal */


/* 
 * ipc_spawn_watchdog -- spawn a watchdog task to clean up the message queue should the
 * server process die.
 */
void
ipc_spawn_watchdog (void)
{
	if ((ipc_wpid = fork ()) == 0) {	/* child process */
		int ppid = getppid ();	/* parent's process id */

		setpgrp ();	/* gnu kills process group on exit */

		while (1) {
			if (kill (ppid, 0) < 0) {	/* ppid is no longer
							 * valid, parent may
							 * have died */
				ipc_exit (0);
			}	/* if */
			sleep (10);	/* have another go later */
		}		/* while */
	}			/* if */
}				/* ipc_spawn_watchdog */


/*
 * ipc_init -- initialize server, setting the global msqid that can be listened on.
 */
void
ipc_init (struct msgbuf **msgpp)
{
	key_t key;		/* messge key */
	char buf[GSERV_BUFSZ];	/* pathname for key */

	sprintf (buf, "/tmp/lgtd%d", (int) geteuid ());
	creat (buf, 0600);
	key = ftok (buf, 1);

	if ((ipc_qid = msgget (key, 0600 | IPC_CREAT)) == -1)
		glibtop_error_io ("unable to create msg queue");

	ipc_spawn_watchdog ();

	signal (SIGTERM, ipc_handle_signal);
	signal (SIGINT, ipc_handle_signal);

	if ((*msgpp = (struct msgbuf *)
	     malloc (sizeof **msgpp + GSERV_BUFSZ)) == NULL) {
		glibtop_warn_io ("unable to allocate space for message buffer");
		ipc_exit (1);
	}			/* if */
}				/* ipc_init */


/*
 * handle_ipc_request -- accept a request from a client, pass the request on
 * to the GNU Emacs process, then wait for its reply and
 * pass that on to the client.
 */
void
handle_ipc_request (struct msgbuf *msgp)
{
#if 0
	struct msqid_ds msg_st;	/* message status */
	char buf[GSERV_BUFSZ];
	int len;		/* length of message / read */
	int s, result_len;	/* tag fields on the response from emacs */
	int offset = 0;
	int total = 1;		/* # bytes that will actually be sent off */

	if ((len = msgrcv (ipc_qid, msgp, GSERV_BUFSZ - 1, 1, 0)) < 0) {
		glibtop_warn_io ("msgrcv");
		ipc_exit (1);
	}			/* if */
	msgctl (ipc_qid, IPC_STAT, &msg_st);
	strncpy (buf, msgp->mtext, len);
	buf[len] = '\0';	/* terminate */

	printf ("%d %s", ipc_qid, buf);
	fflush (stdout);

	/* now for the response from gnu */
	msgp->mtext[0] = '\0';

#if 0
	if ((len = read (0, buf, GSERV_BUFSZ - 1)) < 0) {
		glibtop_warn_io ("read");
		ipc_exit (1);
	}			/* if */
	sscanf (buf, "%d:%[^\n]\n", &junk, msgp->mtext);
#else

	/* read in "n/m:" (n=client fd, m=message length) */

	while (offset < (GSERV_BUFSZ - 1) &&
	       ((len = read (0, buf + offset, 1)) > 0) &&
	       buf[offset] != ':') {
		offset += len;
	}

	if (len < 0)
		glibtop_error_io ("read");

	/* parse the response from emacs, getting client fd & result length */
	buf[offset] = '\0';
	sscanf (buf, "%d/%d", &s, &result_len);

	while (result_len > 0) {
		if ((len = read (0, buf, min2 (result_len, GSERV_BUFSZ - 1))) < 0)
			glibtop_error_io ("read");

		/* Send this string off, but only if we have enough space */

		if (GSERV_BUFSZ > total) {
			if (total + len <= GSERV_BUFSZ)
				buf[len] = 0;
			else
				buf[GSERV_BUFSZ - total] = 0;

			send_string (s, buf);
			total += strlen (buf);
		}
		result_len -= len;
	}

	/* eat the newline */
	while ((len = read (0, buf, 1)) == 0);
	if (len < 0)
		glibtop_error_io ("read");

	if (buf[0] != '\n')
		glibtop_error ("garbage after result [%c]", buf[0]);
#endif

	/* Send a response back to the client. */

	msgp->mtype = msg_st.msg_lspid;
	if (msgsnd (ipc_qid, msgp, strlen (msgp->mtext) + 1, 0) < 0)
		glibtop_warn_io ("msgsend(gnuserv)");
#else
	glibtop_error ("handle_ipc_request (): Function not implemented");
#endif
}				/* handle_ipc_request */
#endif /* SYSV_IPC */


#ifdef INTERNET_DOMAIN_SOCKETS

#ifdef AUTH_MAGIC_COOKIE
#include <X11/X.h>
#include <X11/Xauth.h>

static Xauth *server_xauth = NULL;

#endif

static int
timed_read (int fd, char *buf, int max, int timeout, int one_line)
{
	fd_set rmask;
	struct timeval tv;	/* = {timeout, 0}; */
	char c = 0;
	int nbytes = 0;
	int r;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	FD_ZERO (&rmask);
	FD_SET (fd, &rmask);

	do {
		r = select (fd + 1, &rmask, NULL, NULL, &tv);

		if (r > 0) {
			if (read (fd, &c, 1) == 1) {
				*buf++ = c;
				++nbytes;
			} else {
				glibtop_warn_io ("read error on socket");
				return -1;
			}
		} else if (r == 0) {
			glibtop_warn ("read timed out");
			return -1;
		} else {
			glibtop_warn_io ("error in select");
			return -1;
		}
	} while ((nbytes < max) && !(one_line && (c == '\n')));

	--buf;
	if (one_line && *buf == '\n') {
		*buf = 0;
	}
	return nbytes;
}



/*
 * permitted -- return whether a given host is allowed to connect to the server.
 */
static int
permitted (u_long host_addr, int fd)
{
	int i;

	char auth_protocol[128];
	char buf[1024];
	int auth_data_len;

	if (fd > 0) {
		/* we are checking permission on a real connection */

		/* Read auth protocol name */

		if (timed_read (fd, auth_protocol, AUTH_NAMESZ, AUTH_TIMEOUT, 1) <= 0)
			return FALSE;

#ifdef DEBUG
		fprintf (stderr, "Client sent authenticatin protocol '%s'\n", auth_protocol);
#endif

		if (strcmp (auth_protocol, DEFAUTH_NAME) &&
		    strcmp (auth_protocol, MCOOKIE_NAME)) {
			glibtop_warn ("Authentication protocol from client is invalid", auth_protocol);

			return FALSE;
		}
		if (!strcmp (auth_protocol, MCOOKIE_NAME)) {

			/* 
			 * doing magic cookie auth
			 */

			if (timed_read (fd, buf, 10, AUTH_TIMEOUT, 1) <= 0)
				return FALSE;

			auth_data_len = atoi (buf);

			if (timed_read (fd, buf, auth_data_len, AUTH_TIMEOUT, 0) != auth_data_len)
				return FALSE;

#ifdef AUTH_MAGIC_COOKIE
			if (server_xauth && server_xauth->data &&
			 !memcmp (buf, server_xauth->data, auth_data_len)) {
				return TRUE;
			}
#else
			glibtop_warn ("client tried Xauth, but server is not compiled with Xauth");
#endif

			/* 
			 * auth failed, but allow this to fall through to the GNU_SECURE
			 * protocol....
			 */

			glibtop_warn ("Xauth authentication failed, trying GNU_SECURE auth...");

		}
		/* Other auth protocols go here, and should execute only if
		 * the * auth_protocol name matches. */

	}
	/* Now, try the old GNU_SECURE stuff... */

#ifdef DEBUG
	fprintf (stderr, "Doing GNU_SECURE auth ...\n");
#endif

	/* Now check the chain for that hash key */
	for (i = 0; i < HOST_TABLE_ENTRIES; i++) {
#ifdef DEBUG
		fprintf (stderr, "Trying %lx - %lx\n",
			 host_addr, permitted_hosts [i]);
#endif
		if (host_addr == permitted_hosts [i])
			return (TRUE);
	}

	return (FALSE);
}				/* permitted */


/*
 * setup_table -- initialise the table of hosts allowed to contact the server,
 * by reading from the file specified by the GNU_SECURE
 * environment variable
 * Put in the local machine, and, if a security file is specifed,
 * add each host that is named in the file.
 * Return the number of hosts added.
 */
static int
setup_table (void)
{
	char hostname [HOSTNAMSZ];
	u_int host_addr;
	int i, hosts = 0;

	/* Make sure every entry is null */
	for (i = 0; i < HOST_TABLE_ENTRIES; i++)
		permitted_hosts [i] = 0;

	gethostname (hostname, HOSTNAMSZ);

	if (((long) host_addr = glibtop_internet_addr (hostname)) == -1)
		glibtop_error ("Can't resolve '%s'", hostname);

#ifdef AUTH_MAGIC_COOKIE

	server_xauth = XauGetAuthByAddr
		(FamilyInternet,
		 sizeof (host_addr), (char *) &host_addr,
		 strlen (MCOOKIE_SCREEN), MCOOKIE_SCREEN,
		 strlen (MCOOKIE_X_NAME), MCOOKIE_X_NAME);
	hosts++;

#endif /* AUTH_MAGIC_COOKIE */

	/* Resolv host names from permitted_host_names []. */

	for (i = 0; i < HOST_TABLE_ENTRIES; i++) {
#ifdef DEBUG
		fprintf (stderr, "Resolving %s ...\n",
			 permitted_host_names [i]);
#endif
		permitted_hosts [i] =
			glibtop_internet_addr (permitted_host_names [i]);
		if ((long) permitted_hosts [i] == -1)
			glibtop_error ("Can't resolve '%s'",
				       permitted_host_names [i]);
	}

#ifdef DEBUG
	for (i = 0; i < HOST_TABLE_ENTRIES; i++)
		fprintf (stderr, "Host %s - %lx\n",
			 permitted_host_names [i],
			 permitted_hosts [i]);
#endif

	hosts += HOST_TABLE_ENTRIES;

	return hosts;
}				/* setup_table */


/*
 * internet_init -- initialize server, returning an internet socket that can
 * be listened on.
 */
static int
internet_init (void)
{
	int ls;			/* socket descriptor */
	struct sockaddr_in server;	/* for local socket address */

	if (setup_table () == 0)
		return -1;

	/* clear out address structure */
	memset ((char *) &server, 0, sizeof (struct sockaddr_in));

	/* Set up address structure for the listen socket. */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	/* We use a fixed port given in the config file. */
	server.sin_port = htons (SERVER_PORT);

	fprintf (stderr, "Using port %u.\n", server.sin_port);

	/* Create the listen socket. */
	if ((ls = socket (AF_INET, SOCK_STREAM, 0)) == -1)
		glibtop_error_io ("unable to create socket");
	
	/* Bind the listen address to the socket. */
	if (bind (ls, (struct sockaddr *) &server, sizeof (struct sockaddr_in)) == -1)
		  glibtop_error_io ("bind");

	/* Initiate the listen on the socket so remote users * can connect.  */
	if (listen (ls, 20) == -1)
		glibtop_error_io ("listen");

	return (ls);
}				/* internet_init */


/*
 * handle_internet_request -- accept a request from a client and send the information
 * to stdout (the gnu process).
 */
static void
handle_internet_request (int ls)
{
	int s;
	size_t addrlen = sizeof (struct sockaddr_in);
	struct sockaddr_in peer;	/* for peer socket address */
	pid_t pid;

	memset ((char *) &peer, 0, sizeof (struct sockaddr_in));

	if ((s = accept (ls, (struct sockaddr *) &peer, (void *) &addrlen)) == -1)
		glibtop_error_io ("accept");

#ifdef DEBUG
	fprintf (stderr, "Connection was made from %s.\n", inet_ntoa (peer.sin_addr));
#endif

	/* Check that access is allowed - if not return crud to the client */
	if (!permitted (peer.sin_addr.s_addr, s)) {
		close (s);
		glibtop_warn ("Refused connection from %s.", inet_ntoa (peer.sin_addr));
		return;
	}			/* if */

#ifdef DEBUG
	fprintf (stderr, "Accepted connection from %s (%u) on socket %d.\n",
		 inet_ntoa (peer.sin_addr), ntohs (peer.sin_port), s);
#endif

	pid = fork ();

	if (pid == -1)
		glibtop_error_io ("fork failed");

	if (pid)
		return;

	handle_parent_connection (glibtop_global_server, s);

	close (s);
	
#ifdef DEBUG
	fprintf (stderr, "Closed connection to %s (%d).\n",
		 inet_ntoa (peer.sin_addr), ntohs (peer.sin_port));
#endif

	_exit (0);
}				/* handle_internet_request */
#endif /* INTERNET_DOMAIN_SOCKETS */


#ifdef UNIX_DOMAIN_SOCKETS
/*
 * unix_init -- initialize server, returning an unix-domain socket that can
 * be listened on.
 */
static int
unix_init (void)
{
	int ls;			/* socket descriptor */
	struct sockaddr_un server;	/* unix socket address */
	int bindlen;

	if ((ls = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
		glibtop_error_io ("unable to create socket");

	/* Set up address structure for the listen socket. */

#ifdef HIDE_UNIX_SOCKET
	sprintf (server.sun_path, "/tmp/lgtddir%d", (int) geteuid ());
	if (mkdir (server.sun_path, 0700) < 0) {
		/* assume it already exists, and try to set perms */
		if (chmod (server.sun_path, 0700) < 0)
			glibtop_error_io ("Can't set permissions on %s",
					  server.sun_path);
	}
	strcat (server.sun_path, "/lgtd");
	unlink (server.sun_path);	/* remove old file if it exists */
#else /* HIDE_UNIX_SOCKET */
	sprintf (server.sun_path, "/tmp/lgtd%d", (int) geteuid ());
	unlink (server.sun_path);	/* remove old file if it exists */
#endif /* HIDE_UNIX_SOCKET */

	server.sun_family = AF_UNIX;
#ifdef HAVE_SOCKADDR_SUN_LEN
	/* See W. R. Stevens "Advanced Programming in the Unix Environment"
	 * p. 502 */
	bindlen = (sizeof (server.sun_len) + sizeof (server.sun_family)
		   + strlen (server.sun_path) + 1);
	server.sun_len = bindlen;
#else
	bindlen = strlen (server.sun_path) + sizeof (server.sun_family);
#endif

	if (bind (ls, (struct sockaddr *) &server, bindlen) < 0)
		glibtop_error_io ("bind");

	chmod (server.sun_path, 0700);	/* only this user can send commands */

	if (listen (ls, 20) < 0)
		glibtop_error_io ("listen");

	/* #### there are also better ways of dealing with this when sigvec() 
	 * is present. */
#if  defined (HAVE_SIGPROCMASK)
	{
		sigset_t _mask;

		sigemptyset (&_mask);
		sigaddset (&_mask, SIGPIPE);
		sigprocmask (SIG_BLOCK, &_mask, NULL);
	}
#else
	signal (SIGPIPE, SIG_IGN);	/* in case user kills client */
#endif

	return (ls);
}				/* unix_init */


/*
 * handle_unix_request -- accept a request from a client and send the information
 * to stdout (the gnu process).
 */
static void
handle_unix_request (int ls)
{
	int s;
	size_t len = sizeof (struct sockaddr_un);
	struct sockaddr_un server;	/* for unix socket address */
	pid_t pid;

	server.sun_family = AF_UNIX;

	if ((s = accept (ls, (struct sockaddr *) &server, (void *) &len)) < 0)
		glibtop_error_io ("accept");

#ifdef DEBUG
	fprintf (stderr, "Accepted connection on socket %d.\n", s);
#endif

	pid = fork ();

	if (pid == -1)
		glibtop_error_io ("fork failed");

	if (pid)
		return;

	handle_child_connection (glibtop_global_server, s);

	close (s);

#ifdef DEBUG
	fprintf (stderr, "Closed connection on socket %d.\n", s);
#endif

	glibtop_close_r (glibtop_global_server);

	exit (0);
}				/* handle_unix_request */

#endif /* UNIX_DOMAIN_SOCKETS */

void
handle_signal (int sig)
{
	fprintf (stderr, "Catched signal %d.\n", sig);
}

int
main (int argc, char *argv [])
{
	int ils = -1;		/* internet domain listen socket */
	int uls = -1;		/* unix domain listen socket */
	pid_t pid;

#ifdef SYSV_IPC
	struct msgbuf *msgp;	/* message buffer */

#endif /* SYSV_IPC */

	glibtop_init_r (&glibtop_global_server, 0, GLIBTOP_OPEN_NO_OVERRIDE);

	/* Fork a child.
	 *
	 * The parent will listen for incoming internet connections
	 * and the child will listen for connections from the local
	 * host using unix domain name sockets or SysV IPC.
	 */

	signal (SIGCHLD, handle_signal);

	pid = fork ();

	if (pid == -1)
		glibtop_error_io ("fork failed");
	else if (pid == 0) {
		/* We are the child. */

		/* Temporarily drop our priviledges. */

		fprintf (stderr, "Child ID: (%d, %d) - (%d, %d)\n",
			 getuid (), geteuid (), getgid (), getegid ());

		if (setreuid (geteuid (), getuid ()))
			glibtop_error_io ("setreuid (euid <-> uid)");
		
		if (setregid (getegid (), getgid ()))
			glibtop_error_io ("setregid (egid <-> gid)");
		
		fprintf (stderr, "Child ID: (%d, %d) - (%d, %d)\n",
			 getuid (), geteuid (), getgid (), getegid ());

#ifdef SYSV_IPC
		/* get a msqid to listen on, and a message buffer. */
		ipc_init (&msgp);
#endif /* SYSV_IPC */

#ifdef UNIX_DOMAIN_SOCKETS
		/* get a unix domain socket to listen on. */
		uls = unix_init ();
#endif /* UNIX_DOMAIN_SOCKETS */
	} else {
		/* We are the parent. */

		/* If we are root, completely switch to SERVER_UID and
		 * SERVER_GID. Otherwise we completely drop any priviledges.
		 */
		
#ifdef DEBUG
		fprintf (stderr, "Parent ID: (%d, %d) - (%d, %d)\n",
			 getuid (), geteuid (), getgid (), getegid ());
#endif

		if (setreuid (geteuid (), getuid ()))
			glibtop_error_io ("setreuid (euid <-> uid)");
		
		if (setregid (getegid (), getgid ()))
			glibtop_error_io ("setregid (egid <-> gid)");
		
#ifdef DEBUG
		fprintf (stderr, "Parent ID: (%d, %d) - (%d, %d)\n",
			 getuid (), geteuid (), getgid (), getegid ());
#endif

		if ((geteuid () == 0) || (getuid () == 0)) {
			if (setreuid (0, 0))
				glibtop_error_io ("setreuid (root)");
		}

#ifdef DEBUG
		fprintf (stderr, "Parent ID: (%d, %d) - (%d, %d)\n",
			 getuid (), geteuid (), getgid (), getegid ());
#endif

		if (geteuid () == 0) {
			if (setregid (SERVER_GID, SERVER_GID))
				glibtop_error_io ("setregid (SERVER_GID)");
			if (setreuid (SERVER_UID, SERVER_UID))
				glibtop_error_io ("setreuid (SERVER_UID)");
		} else {
			if (setreuid (geteuid (), geteuid ()))
				glibtop_error_io ("setreuid (euid)");
		}

#ifdef DEBUG
		fprintf (stderr, "Parent ID: (%d, %d) - (%d, %d)\n",
			 getuid (), geteuid (), getgid (), getegid ());
#endif

#ifdef INTERNET_DOMAIN_SOCKETS
		/* get a internet domain socket to listen on. */
		ils = internet_init ();
#endif /* INTERNET_DOMAIN_SOCKETS */
	}

	while (1) {
#ifdef SYSV_IPC
		handle_ipc_request (msgp);
#else /* NOT SYSV_IPC */
		fd_set rmask;
		int ret;

		while ((ret = wait3 (NULL, WNOHANG, NULL)) != 0) {
			if ((ret == -1) && (errno == ECHILD))
				break;

			if ((ret == -1) && ((errno == EAGAIN) ||
					    (errno == ERESTART)))
				continue;
			if (ret > 0)
				fprintf (stderr, "Child %d exited.\n", ret);
			else
				glibtop_warn_io ("wait3: %d", ret);
		}
		
		FD_ZERO (&rmask);

		/* Only the child accepts connections from standard
		 * input made by its parent. */

		if (pid == 0)
			FD_SET (fileno (stdin), &rmask);

		if (uls >= 0)
			FD_SET (uls, &rmask);
		if (ils >= 0)
			FD_SET (ils, &rmask);

#ifdef DEBUG
		fprintf (stderr, "Server ready and waiting for connections.\n");
#endif

		if (select (max2 (fileno (stdin), max2 (uls, ils)) + 1, &rmask,
			    (fd_set *) NULL, (fd_set *) NULL,
			    (struct timeval *) NULL) < 0) {
			if (errno == EINTR)
				continue;
			glibtop_error_io ("select");
		}

#ifdef UNIX_DOMAIN_SOCKETS
		if (uls > 0 && FD_ISSET (uls, &rmask))
			handle_unix_request (uls);
#endif

#ifdef INTERNET_DOMAIN_SOCKETS
		if (ils > 0 && FD_ISSET (ils, &rmask))
			handle_internet_request (ils);
#endif /* INTERNET_DOMAIN_SOCKETS */

		if (FD_ISSET (fileno (stdin), &rmask))
			handle_child_connection (glibtop_global_server, fileno (stdin));
#endif /* NOT SYSV_IPC */
	}			/* while */

	return 0;
}				/* main */

#endif /* SYSV_IPC || UNIX_DOMAIN_SOCKETS || INTERNET_DOMAIN_SOCKETS */
