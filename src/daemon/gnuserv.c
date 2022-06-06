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

#include <config.h>

#include <glib/gi18n-lib.h>
#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/command.h>
#include <glibtop/parameter.h>

#include "server_config.h"

#include <glibtop/gnuserv.h>

#include <errno.h>
#include <locale.h>

#include "daemon.h"

#ifdef AIX
#include <sys/select.h>
#endif

#ifdef NEED_DECLARATION_PROGRAM_INVOCATION_NAME
extern char *program_invocation_name, *program_invocation_short_name;
#endif

#ifndef HAVE_PROGRAM_INVOCATION_SHORT_NAME
char *program_invocation_short_name;
#endif

#ifndef HAVE_PROGRAM_INVOCATION_NAME
char *program_invocation_name;
#endif


#if !defined(INTERNET_DOMAIN_SOCKETS)
#error "Internet Domain sockets are required"
#endif

#ifdef AUTH_MAGIC_COOKIE
#include <X11/X.h>
#include <X11/Xauth.h>

static Xauth *server_xauth = NULL;

#endif /* AUTH_MAGIC_COOKIE */

gboolean enable_debug = FALSE;
gboolean verbose_output = FALSE;
static gboolean no_daemon = FALSE;
static gboolean invoked_from_inetd = FALSE;
static int changed_uid = 0;

void
syslog_message (int priority, const char *format, ...)
{
    va_list ap;
    char buffer [BUFSIZ];

    va_start (ap, format);
    vsnprintf (buffer, BUFSIZ-1, format, ap);
    va_end (ap);

    syslog (priority, "%s", buffer);
}

void
syslog_io_message (int priority, const char *format, ...)
{
    va_list ap;
    char buffer [BUFSIZ];
    char buffer2 [BUFSIZ];

    va_start (ap, format);
    vsnprintf (buffer, BUFSIZ-1, format, ap);
    va_end (ap);

    snprintf (buffer2, BUFSIZ-1, "%s: %s", buffer, g_strerror (errno));
    syslog (priority, "%s", buffer2);
}

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
		syslog_io_message (LOG_WARNING, "read error on socket");
		return -1;
	    }
	} else if (r == 0) {
	    syslog_io_message (LOG_WARNING, "read timed out");
	    return -1;
	} else {
	    syslog_io_message (LOG_WARNING, "error in select");
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

    /* Read auth protocol name */

    if (timed_read (fd, auth_protocol, AUTH_NAMESZ, AUTH_TIMEOUT, 1) <= 0)
	return FALSE;

    if (enable_debug)
	syslog_message (LOG_DEBUG,
			"Client sent authenticatin protocol '%s'.",
			auth_protocol);

    if (strcmp (auth_protocol, DEFAUTH_NAME) &&
	strcmp (auth_protocol, MCOOKIE_NAME)) {
	syslog_message (LOG_WARNING,
			"Invalid authentication protocol "
			"'%s' from client",
			auth_protocol);
	return FALSE;
    }

    if (!strcmp (auth_protocol, MCOOKIE_NAME)) {
	/*
	 * doing magic cookie auth
	 */

	if (timed_read (fd, buf, 10, AUTH_TIMEOUT, 1) <= 0)
	    return FALSE;

	auth_data_len = atoi (buf);

	if (auth_data_len < 1 || (size_t)auth_data_len > sizeof(buf)) {
	    syslog_message(LOG_WARNING, "Invalid data length supplied by client");
	    return FALSE;
	}

	if (timed_read (fd, buf, auth_data_len, AUTH_TIMEOUT, 0) != auth_data_len)
	    return FALSE;

#ifdef AUTH_MAGIC_COOKIE
	if (!invoked_from_inetd && server_xauth && server_xauth->data &&
	    !memcmp (buf, server_xauth->data, auth_data_len)) {
	    return TRUE;
	}
#else
	syslog_message (LOG_WARNING,
			"Client tried Xauth, but server is "
			"not compiled with Xauth");
#endif

	/*
	 * auth failed, but allow this to fall through to the
	 * GNU_SECURE protocol....
	 */

	if (verbose_output) {
	    if (changed_uid || invoked_from_inetd)
		syslog_message (LOG_WARNING,
				"Xauth authentication not allowed, "
				"trying GNU_SECURE ...");
	    else
		syslog_message (LOG_WARNING,
				"Xauth authentication failed, "
				"trying GNU_SECURE auth...");
	}
    }

    /* Other auth protocols go here, and should execute only if
     * the * auth_protocol name matches. */

    /* Now, try the old GNU_SECURE stuff... */

    if (enable_debug)
	syslog_message (LOG_DEBUG, "Doing GNU_SECURE auth ...");

    /* Now check the chain for that hash key */
    for (i = 0; i < HOST_TABLE_ENTRIES; i++) {
	if (enable_debug)
	    syslog_message (LOG_DEBUG, "Trying %lx - %lx",
			    host_addr, permitted_hosts [i]);
	if (permitted_hosts [i] == 0L)
	    return (FALSE);
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

#ifdef AUTH_MAGIC_COOKIE
    char screen [BUFSIZ];
#endif

    long host_addr;
    int i, hosts = 0;

    /* Make sure every entry is null */
    for (i = 0; i < HOST_TABLE_ENTRIES; i++)
	permitted_hosts [i] = 0;

    gethostname (hostname, HOSTNAMSZ);

    if ((host_addr = glibtop_internet_addr (hostname)) == -1) {
	syslog_io_message (LOG_ERR, "Can't resolve '%s'", hostname);
	exit (1);
    }

#ifdef AUTH_MAGIC_COOKIE

    sprintf (screen, "%d", SERVER_PORT);

    server_xauth = XauGetAuthByAddr
	(FamilyInternet,
	 sizeof (host_addr), (char *) &host_addr,
	 strlen (screen), screen,
	 strlen (MCOOKIE_X_NAME), MCOOKIE_X_NAME);
    hosts++;

#endif /* AUTH_MAGIC_COOKIE */

    /* Resolv host names from permitted_host_names []. */

    for (i = 0; i < HOST_TABLE_ENTRIES; i++) {
	if (!permitted_host_names [i])
	    continue;
	if (enable_debug)
	    syslog_message (LOG_DEBUG, "Resolving %s ...",
			    permitted_host_names [i]);
	permitted_hosts [i] =
	    glibtop_internet_addr (permitted_host_names [i]);
	if ((long) permitted_hosts [i] == -1) {
	    syslog_io_message (LOG_ERR, "Can't resolve '%s'",
			       permitted_host_names [i]);
	    exit (1);
	}
    }

    if (enable_debug)
	for (i = 0; i < HOST_TABLE_ENTRIES; i++)
	    syslog_message (LOG_DEBUG, "Host %s - %lx",
			    permitted_host_names [i],
			    permitted_hosts [i]);

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

    if (verbose_output)
	syslog_message (LOG_INFO, "Using port %u.", SERVER_PORT);

    /* Create the listen socket. */
    if ((ls = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
	syslog_io_message (LOG_ERR, "unable to create socket");
	exit (1);
    }

    /* Bind the listen address to the socket. */
    if (bind (ls, (struct sockaddr *) &server,
	      sizeof (struct sockaddr_in)) == -1) {
	syslog_io_message (LOG_ERR, "bind");
	exit (1);
    }

    /* Initiate the listen on the socket so remote users * can connect.  */
    if (listen (ls, 20) == -1) {
	syslog_io_message (LOG_ERR, "listen");
	exit (1);
    }

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
    char addrstr[addrlen];
    pid_t pid;

    memset ((char *) &peer, 0, sizeof (struct sockaddr_in));

    if ((s = accept (ls, (struct sockaddr *) &peer, (void *) &addrlen)) == -1) {
	syslog_io_message (LOG_ERR, "accept");
	exit (1);
    }

    /* TODO: Check errno. */
    inet_ntop (AF_INET, &peer, addrstr, addrlen);

    if (verbose_output)
	syslog_message (LOG_INFO, "Connection was made from %s port %u.",
			addrstr, ntohs (peer.sin_port));

    /* Check that access is allowed - if not return crud to the client */
    if (!permitted (peer.sin_addr.s_addr, s)) {
	close (s);
	syslog_message (LOG_CRIT, "Refused connection from %s.",
			addrstr);
	return;
    }			/* if */

    if (verbose_output)
	syslog_message (LOG_INFO, "Accepted connection from %s port %u.",
			addrstr, ntohs (peer.sin_port));

    pid = fork ();

    if (pid == -1) {
	syslog_io_message (LOG_ERR, "fork failed");
	exit (1);
    }

    if (pid) {
	if (verbose_output)
	    syslog_message (LOG_INFO, "Child pid is %d.", pid);
	return;
    }

    handle_parent_connection (s);

    close (s);

    if (verbose_output)
	syslog_message (LOG_INFO, "Closed connection to %s port %u.",
			addrstr, ntohs (peer.sin_port));

    _exit (0);
}				/* handle_internet_request */

static void
handle_signal (int sig)
{
    if (sig == SIGCHLD)
	return;

    syslog_message (LOG_ERR, "Catched signal %d.\n", sig);
    exit (1);
}

static const GOptionEntry options [] = {
    { "debug", 'd', 0, G_OPTION_ARG_NONE, &enable_debug,
      N_("Enable debugging"), NULL },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose_output,
      N_("Enable verbose output"), NULL },
    { "no-daemon", 'f', 0, G_OPTION_ARG_NONE, &no_daemon,
      N_("Don’t fork into background"), NULL },
    { "inetd", 'i', 0, G_OPTION_ARG_NONE, &invoked_from_inetd,
      N_("Invoked from inetd"), NULL },
    { NULL }
};

int
main (int argc, char **argv)
{
    const unsigned method = GLIBTOP_METHOD_PIPE;
    const unsigned long features = GLIBTOP_SYSDEPS_ALL;
    glibtop *server = glibtop_global_server;
    GOptionContext *goption_context;
    GError *error = NULL;

    int ils = -1;		/* internet domain listen socket */

    setlocale (LC_ALL, "");

    /* On non-glibc systems, this is not set up for us.  */
    if (!program_invocation_name) {
	char *arg;

	program_invocation_name = (char *) argv[0];
	arg = strrchr (argv[0], '/');
	program_invocation_short_name =
	    arg ? (arg + 1) : program_invocation_name;
    }

    g_set_prgname (program_invocation_short_name);
    goption_context = g_option_context_new (NULL);
    g_option_context_add_main_entries (goption_context, options, NULL);
    g_option_context_parse (goption_context, &argc, &argv, &error);
    g_option_context_free (goption_context);

    if (error != NULL) {
        g_printerr ("%s\n", error->message);
        g_error_free (error);
        g_printerr (_("Run “%s --help” to see a full list of "
                    "available command line options.\n"),
                    program_invocation_name);
        exit(1);
    }

    if (enable_debug)
	verbose_output = 1;

    if (no_daemon) {
	openlog ("libgtop-daemon", LOG_PERROR | LOG_PID, LOG_LOCAL0);
    } else {
	openlog ("libgtop-daemon", LOG_PID, LOG_LOCAL0);
    }

    if (!no_daemon && !invoked_from_inetd) {
	pid_t pid = fork ();

	if (pid == -1) {
	    syslog_io_message (LOG_ERR, "fork failed");
	    exit (1);
	} else if (pid)
	    exit (0);

	close (0);

	setsid ();
    }

    glibtop_init_r (&glibtop_global_server, 0, GLIBTOP_INIT_NO_INIT);

    signal (SIGCHLD, handle_signal);

    /* If we are root, completely switch to SERVER_UID and
     * SERVER_GID. Otherwise we completely drop any priviledges.
     */

    if (enable_debug)
	syslog_message (LOG_DEBUG, "Parent ID: (%d, %d) - (%d, %d)",
			getuid (), geteuid (), getgid (), getegid ());

    if (geteuid () == 0) {
	changed_uid = 1;
	if (setregid (SERVER_GID, SERVER_GID)) {
	    syslog_io_message (LOG_ERR, "setregid (SERVER_GID)");
	    exit (1);
	}
	if (setreuid (SERVER_UID, SERVER_UID)) {
	    syslog_io_message (LOG_ERR, "setreuid (SERVER_UID)");
	    exit (1);
	}
    } else {
	if (setreuid (geteuid (), geteuid ())) {
	    syslog_io_message (LOG_ERR, "setreuid (euid)");
	    exit (1);
	}
    }

    if (enable_debug)
	syslog_message (LOG_DEBUG, "Parent ID: (%d, %d) - (%d, %d)",
			getuid (), geteuid (), getgid (), getegid ());

    if (invoked_from_inetd) {
	size_t addrlen = sizeof (struct sockaddr_in);
	struct sockaddr_in peer;
	char addrstr[addrlen];

	memset ((char *) &peer, 0, sizeof (struct sockaddr_in));

	if (getpeername (0, (struct sockaddr *) &peer, (void *) &addrlen)) {
	    syslog_io_message (LOG_ERR, "getpeername");
	    exit (1);
	}

	/* TODO: Check errno. */
	inet_ntop (AF_INET, &peer, addrstr, addrlen);

	if (verbose_output)
	    syslog_message (LOG_INFO, "Connection was made from %s port %u.",
			    addrstr, ntohs (peer.sin_port));

	/* Check that access is allowed - if not return crud to the client */
	if (!permitted (peer.sin_addr.s_addr, 0)) {
	    close (0);
	    syslog_message (LOG_CRIT, "Refused connection from %s.",
			    addrstr);
	    exit (1);
	}

	handle_parent_connection (0);
	exit (0);
    }

    /* get a internet domain socket to listen on. */
    ils = internet_init ();

    if (ils <= 0) {
	syslog_message (LOG_ERR, "Unable to get internet domain socket.");
	exit (1);
    }

    glibtop_set_parameter_l (server, GLIBTOP_PARAM_METHOD,
			     &method, sizeof (method));

    server->features = features;

    glibtop_init_r (&server, 0, 0);

    while (1) {
	fd_set rmask;
	int status, ret;

	while ((ret = wait3 (&status, WNOHANG, NULL)) != 0) {
	    if ((ret == -1) && (errno == ECHILD))
		break;

	    if ((ret == -1) && ((errno == EAGAIN)))
		continue;
	    if (ret == 0) {
		syslog_io_message (LOG_WARNING, "wait3");
		continue;
	    }

	    if (verbose_output)
		syslog_message (LOG_INFO, "Child %d exited.", ret);
	}

	FD_ZERO (&rmask);

	/* Only the child accepts connections from standard
	 * input made by its parent. */

	FD_SET (ils, &rmask);

	if (enable_debug)
	    syslog_message (LOG_DEBUG,
			    "Server ready and waiting for connections.");

	if (select (ils+1, &rmask, (fd_set *) NULL, (fd_set *) NULL,
		    (struct timeval *) NULL) < 0) {
	    if (errno == EINTR)
		continue;
	    syslog_io_message (LOG_ERR, "select");
	    exit (1);
	}

	if (FD_ISSET (ils, &rmask))
	    handle_internet_request (ils);
    }

    return 0;
}
