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

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/command.h>
#include <glibtop/xmalloc.h>

#include "server_config.h"

#include <glibtop/gnuserv.h>

#include <fcntl.h>
#include <sys/wait.h>

#ifdef AIX
#include <sys/select.h>
#endif

extern void handle_parent_connection __P ((int));
extern void handle_child_connection __P ((int));
extern void handle_ipc_connection __P ((int));

#if !defined(UNIX_DOMAIN_SOCKETS) && !defined(INTERNET_DOMAIN_SOCKETS)
#error "Unix Domain sockets or Internet Domain sockets are required"
#endif

#ifdef INTERNET_DOMAIN_SOCKETS

#ifdef AUTH_MAGIC_COOKIE
#include <X11/X.h>
#include <X11/Xauth.h>

static Xauth *server_xauth = NULL;

#endif /* INTERNET_DOMAIN_SOCKETS */

/*
 * timed_read - Read with timeout.
 */

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
		fprintf (stderr, "Client sent authenticatin protocol '%s'\n",
			 auth_protocol);
#endif

		if (strcmp (auth_protocol, DEFAUTH_NAME) &&
		    strcmp (auth_protocol, MCOOKIE_NAME)) {
			glibtop_warn ("Invalid authentication protocol "
				      "'%s' from client", auth_protocol);
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
			glibtop_warn ("Client tried Xauth, but server is "
				      "not compiled with Xauth");
#endif

			/* 
			 * auth failed, but allow this to fall through to the
			 * GNU_SECURE protocol....
			 */

			glibtop_warn ("Xauth authentication failed, "
				      "trying GNU_SECURE auth...");
			
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
}


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
 * handle_internet_request -- accept a request from a client and send the
 * information to stdout (the gnu process).
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

	handle_parent_connection (s);

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

	handle_child_connection (s);

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

	glibtop_init_r (&glibtop_global_server, 0, GLIBTOP_OPEN_NO_OVERRIDE);

	/* Fork a child.
	 *
	 * The parent will listen for incoming internet connections
	 * and the child will listen for connections from the local
	 * host using unix domain name sockets.
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

#ifdef UNIX_DOMAIN_SOCKETS
		/* get a unix domain socket to listen on. */
		uls = unix_init ();
#endif
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
#endif
	}

	while (1) {
		fd_set rmask;
		int ret;

		while ((ret = wait3 (NULL, WNOHANG, NULL)) != 0) {
			if ((ret == -1) && (errno == ECHILD))
				break;

			if ((ret == -1) && ((errno == EAGAIN)))
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
#endif

		if (FD_ISSET (fileno (stdin), &rmask))
			handle_child_connection (fileno (stdin));
	}

	return 0;
}
