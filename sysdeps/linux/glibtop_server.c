#include <glibtop.h>
#include <glibtop/error.h>

#include <glib.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <fcntl.h>

/* gcc warning bug */
unsigned get_pageshift(void);


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


/*
 * Read functions
 */
enum TRY_FILE_TO_BUFFER
{
	TRY_FILE_TO_BUFFER_OK = 0,
	TRY_FILE_TO_BUFFER_OPEN = -1,
	TRY_FILE_TO_BUFFER_READ = -2
};

int try_file_to_buffer(char *buffer, const char *format, ...)
{
	char path[4096];
	int fd;
	ssize_t len;
	va_list pa;

	va_start(pa, format);

	/* C99 also provides vsnprintf */
	g_vsnprintf(path, sizeof path, format, pa);

	va_end(pa);

	if((fd = open (path, O_RDONLY)) < 0)
		return TRY_FILE_TO_BUFFER_OPEN;

	len = read (fd, buffer, BUFSIZ-1);
	close (fd);

	if (len < 0)
		return TRY_FILE_TO_BUFFER_READ;

	buffer [len] = '\0';

	return TRY_FILE_TO_BUFFER_OK;
}


void
file_to_buffer(glibtop *server, char *buffer, const char *filename)
{
	switch(try_file_to_buffer(buffer, filename))
	{
	case TRY_FILE_TO_BUFFER_OPEN:
		glibtop_error_io_r (server, "open (%s)", filename);
	case TRY_FILE_TO_BUFFER_READ:
		glibtop_error_io_r (server, "read (%s)", filename);
	}
}


#warning "Ignore the following warning"
unsigned get_pageshift(void)
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
