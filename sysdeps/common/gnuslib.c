/* -*-C-*-
 * Common library code for the GNU Emacs server and client.
 *
 * This file is part of GNU Emacs.
 *
 * Copying is permitted under those conditions described by the GNU
 * General Public License.
 *
 * Copyright (C) 1989 Free Software Foundation, Inc.
 *
 * Author: Andy Norman (ange@hplb.hpl.hp.com), based on
 * 'etc/server.c' and 'etc/emacsclient.c' from the 18.52 GNU
 * Emacs distribution.
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

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/gnuserv.h>

#include <fcntl.h>

#ifdef UNIX_DOMAIN_SOCKETS
static int connect_to_unix_server (void);
#endif

#ifdef INTERNET_DOMAIN_SOCKETS
static int connect_to_internet_server (const char *serverhost, u_short port);
#endif

/* On some systems, e.g. DGUX, inet_addr returns a 'struct in_addr'. */
#ifdef HAVE_BROKEN_INET_ADDR
#define IN_ADDR struct in_addr
#define NUMERIC_ADDR_ERROR (numeric_addr.s_addr == 0xffffffff)
#else
#if (LONGBITS > 32)
#define IN_ADDR unsigned int
#else
#define IN_ADDR unsigned long
#endif
#define NUMERIC_ADDR_ERROR (numeric_addr == (IN_ADDR) 0xffffffff)
#endif

#include <arpa/inet.h>

int
glibtop_make_connection (const char *hostarg, int portarg, int *s)
{
#ifdef INTERNET_DOMAIN_SOCKETS
	char *ptr;

	if (hostarg == NULL)
		hostarg = getenv ("LIBGTOP_HOST");
	if (portarg == 0 && (ptr = getenv ("LIBGTOP_PORT")) != NULL)
		portarg = atoi (ptr);
#endif

	if (hostarg != NULL) {
		/* hostname was given explicitly, via cmd line arg or
		 * LIBGTOP_HOST,  * so obey it. */
#ifdef UNIX_DOMAIN_SOCKETS
		if (!strcmp (hostarg, "unix")) {
			*s = connect_to_unix_server ();
			return (int) CONN_UNIX;
		}
#endif /* UNIX_DOMAIN_SOCKETS */
#ifdef INTERNET_DOMAIN_SOCKETS
		*s = connect_to_internet_server (hostarg, portarg);
		return (int) CONN_INTERNET;
#endif
	} else {
		/* no hostname given.  Use unix-domain/sysv-ipc, or *
		 * internet-domain connection to local host if they're not
		 * available. */
#if   defined(UNIX_DOMAIN_SOCKETS)
		*s = connect_to_unix_server ();
		return (int) CONN_UNIX;
#elif defined(INTERNET_DOMAIN_SOCKETS)
		{
			char localhost[HOSTNAMSZ];

			gethostname (localhost, HOSTNAMSZ);	/* use this
								 * host by
								 * default */
			*s = connect_to_internet_server (localhost, portarg);
			return (int) CONN_INTERNET;
		}
#endif /* IPC type */
	}
}

#if defined(INTERNET_DOMAIN_SOCKETS) || defined(UNIX_DOMAIN_SOCKETS)
/*
 * send_string -- send string to socket.
 */
#if 0
static void
send_string (s, msg)
     int s;
     const char *msg;
{
#if 0
	if (send (s, msg, strlen (msg), 0) < 0) {
		perror (progname);
		fprintf (stderr, "%s: unable to send\n", progname);
		exit (1);
	};			/* if */
#else
	int len, left = strlen (msg);

	while (left > 0) {
		if ((len = write (s, msg, min2 (left, GSERV_BUFSZ))) < 0) {
			/* XEmacs addition: robertl@arnet.com */
			if (errno == EPIPE) {
				return;
			}
			perror (progname);
			fprintf (stderr, "%s: unable to send\n", progname);
			exit (1);
		};		/* if */
		left -= len;
		msg += len;
	};			/* while */
#endif
}				/* send_string */

/*
 * read_line -- read a \n terminated line from a socket
 */
static int
read_line (int s, char *dest)
{
	int length;
	int offset = 0;
	char buffer[GSERV_BUFSZ + 1];

	while ((length = read (s, buffer + offset, 1) > 0) && buffer[offset] != '\n'
	       && buffer[offset] != EOT_CHR) {
		offset += length;
		if (offset >= GSERV_BUFSZ)
			break;
	}
	buffer[offset] = '\0';
	strcpy (dest, buffer);
	return 1;
}				/* read_line */
#endif
#endif /* INTERNET_DOMAIN_SOCKETS || UNIX_DOMAIN_SOCKETS */


#ifdef UNIX_DOMAIN_SOCKETS
/*
 * connect_to_unix_server -- establish connection with server process via a unix-
 * domain socket. Returns socket descriptor for server
 * if successful.
 */
static int
connect_to_unix_server (void)
{
	int s;			/* connected socket descriptor */
	struct sockaddr_un server;	/* for unix connections */

	if ((s = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
		glibtop_error_io ("unable to create socket");

	server.sun_family = AF_UNIX;
#ifdef HIDE_UNIX_SOCKET
	sprintf (server.sun_path, "/tmp/lgtddir%d/lgtd", (int) geteuid ());
#else /* HIDE_UNIX_SOCKET */
	sprintf (server.sun_path, "/tmp/lgtd%d", (int) geteuid ());
#endif /* HIDE_UNIX_SOCKET */
	if (connect (s, (struct sockaddr *)&server, strlen (server.sun_path) + 2) < 0)
		glibtop_error_io ("unable to connect to local");

	return (s);

}				/* connect_to_unix_server */
#endif /* UNIX_DOMAIN_SOCKETS */


#ifdef INTERNET_DOMAIN_SOCKETS
/*
 * internet_addr -- return the internet addr of the hostname or
 * internet address passed. Return -1 on error.
 */
long
glibtop_internet_addr (const char *host)
{
	/* specify IPv4 and TCP */
	struct addrinfo hints = { AF_INET, SOCK_STREAM, };
	struct addrinfo *result;/* pointer to host info for remote host */
	IN_ADDR numeric_addr;	/* host address */

	if (getaddrinfo (NULL, host, &hints, &result) == 0) {
		/* Take only the first address. */
		struct sockaddr_in *res = (struct sockaddr_in *)result->ai_addr;
		numeric_addr = res->sin_addr.s_addr;
		freeaddrinfo (result);
		return numeric_addr;
	}
	else {
		glibtop_warn_io ("getaddrinfo (%s)", host);
		return -1;
	}

}				/* glibtop_internet_addr  */

#ifdef AUTH_MAGIC_COOKIE
#include <X11/X.h>
#include <X11/Xauth.h>

static Xauth *server_xauth = NULL;

#endif

/*
 * connect_to_internet_server -- establish connection with server process via
 * an internet domain socket. Returns socket
 * descriptor for server if successful.
 */
static int
connect_to_internet_server (const char *serverhost, u_short port)
{
	int s;			/* connected socket descriptor */
	struct servent *sp;	/* pointer to service information */
	struct sockaddr_in peeraddr_in;		/* for peer socket address */
	char buf[512];		/* temporary buffer */

	/* clear out address structures */
	memset (&peeraddr_in, 0, sizeof (struct sockaddr_in));

	/* Set up the peer address to which we will connect. */
	peeraddr_in.sin_family = AF_INET;

	/* look up the server host's internet address */
	peeraddr_in.sin_addr.s_addr = glibtop_internet_addr (serverhost);
	if ((long) peeraddr_in.sin_addr.s_addr == -1)
		glibtop_error ("unable to find %s in /etc/hosts or from YP", serverhost);

	if (port == 0) {
		if ((sp = getservbyname ("gtopd", "tcp")) == NULL)
			peeraddr_in.sin_port = htons (DEFAULT_PORT + getuid ());
		else
			peeraddr_in.sin_port = sp->s_port;
	}
	/* if */
	else
		peeraddr_in.sin_port = htons (port);

	/* Create the socket. */
	if ((s = socket (AF_INET, SOCK_STREAM, 0)) == -1)
		glibtop_error_io ("unable to create socket");

	/* Try to connect to the remote server at the address * which was
	 * just built into peeraddr. */
	if (connect (s, (struct sockaddr *) &peeraddr_in,
		     sizeof (struct sockaddr_in)) == -1)
		  glibtop_error_io ("unable to connect to remote");

#ifdef AUTH_MAGIC_COOKIE

	/* send credentials using MIT-MAGIC-COOKIE-1 protocol */

	sprintf (buf, "%d", port);

	server_xauth =
		XauGetAuthByAddr (FamilyInternet,
				  sizeof (peeraddr_in.sin_addr.s_addr),
				  (char *) &peeraddr_in.sin_addr.s_addr,
				  strlen (buf), buf,
				  strlen (MCOOKIE_X_NAME), MCOOKIE_X_NAME);

	if (server_xauth && server_xauth->data) {
		sprintf (buf, "%s\n%d\n", MCOOKIE_NAME, server_xauth->data_length);
		write (s, buf, strlen (buf));
		write (s, server_xauth->data, server_xauth->data_length);

		return (s);
	}
#endif /* AUTH_MAGIC_COOKIE */

	sprintf (buf, "%s\n", DEFAUTH_NAME);
	write (s, buf, strlen (buf));

	return (s);

}				/* connect_to_internet_server */
#endif /* INTERNET_DOMAIN_SOCKETS */


#if defined(INTERNET_DOMAIN_SOCKETS) || defined(UNIX_DOMAIN_SOCKETS)
/*
 * disconnect_from_server -- inform the server that sending has finished, and wait for
 * its reply.
 */
#if 0
static void
disconnect_from_server (s, echo)
     int s;
     int echo;
{
#if 0
	char buffer[REPLYSIZ + 1];

#else
	char buffer[GSERV_BUFSZ + 1];

#endif
	int add_newline = 1;
	int length;

	send_string (s, EOT_STR);	/* make sure server gets string */

#if !defined (linux)  && !defined (_SCO_DS)
	/*
	 * shutdown is completely hozed under linux. If s is a unix domain socket,
	 * you'll get EOPNOTSUPP back from it. If s is an internet socket, you get
	 * a broken pipe when you try to read a bit later. The latter
	 * problem is fixed for linux versions >= 1.1.46, but the problem
	 * with unix sockets persists. Sigh.
	 */

	if (shutdown (s, 1) == -1) {
		perror (progname);
		fprintf (stderr, "%s: unable to shutdown socket\n", progname);
		exit (1);
	};			/* if */
#endif

#if 0
	while ((length = recv (s, buffer, REPLYSIZ, 0)) > 0) {
		buffer[length] = '\0';
		if (echo)
			fputs (buffer, stdout);
		add_newline = (buffer[length - 1] != '\n');
	};			/* while */
#else
	while ((length = read (s, buffer, GSERV_BUFSZ)) > 0 ||
	       (length == -1 && errno == EINTR)) {
		if (length) {
			buffer[length] = '\0';
			if (echo) {
				fputs (buffer, stdout);
				add_newline = (buffer[length - 1] != '\n');
			};	/* if */
		};		/* if */
	};			/* while */
#endif

	if (echo && add_newline)
		putchar ('\n');

	if (length < 0) {
		perror (progname);
		fprintf (stderr, "%s: unable to read the reply from the server\n", progname);
		exit (1);
	};			/* if */

}				/* disconnect_from_server */
#endif
#endif /* INTERNET_DOMAIN_SOCKETS || UNIX_DOMAIN_SOCKETS */
