#include <glibtop.h>

#include <glib.h>

#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

/* gcc warning bug */
unsigned get_pageshift();


unsigned long long
get_scaled(const char *buffer, const char *key)
{
	const char    *ptr;
	char	      *next;
	unsigned long long value = 0;

	if ((ptr = strstr(buffer, key)))
	{
		ptr += strlen(key);
		value = strtoull(ptr, &next, 0);
		if (strchr(next, 'k'))
			value *= 1024;
		else if (strchr(next, 'M'))
			value *= 1024 * 1024;
	}
	return value;
}


int
proc_file_to_buffer (char *buffer, const char *fmt, pid_t pid)
{
	char filename [256];
	int fd;
	ssize_t len;

	g_snprintf (filename, sizeof filename, fmt, pid);

	fd = open (filename, O_RDONLY);
	if (fd < 0) return -1;

	len = read (fd, buffer, BUFSIZ-1);
	close (fd);

	if (len < 0) return -1;

	buffer [len] = '\0';

	return 0;
}

#warning "Ignore the following warning"
unsigned get_pageshift()
{
	static unsigned pageshift = 0;

	if(G_UNLIKELY(!pageshift))
	{
		register unsigned pagesize = getpagesize();

		while( pagesize > 1 )
		{
			pagesize >>= 1;
			pageshift++;
		}
	}

	return pageshift;
}
