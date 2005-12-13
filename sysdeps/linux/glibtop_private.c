#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>

#include "glibtop_private.h"

#include <glib.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <fcntl.h>
#include <unistd.h>


unsigned long long
get_scaled(const char *buffer, const char *key)
{
	const char    *ptr;
	char	      *next;
	unsigned long long value = 0;

	if (G_LIKELY((ptr = strstr(buffer, key))))
	{
		ptr += strlen(key);
		value = strtoull(ptr, &next, 0);
		if (strchr(next, 'k'))
			value *= 1024;
		else if (strchr(next, 'M'))
			value *= 1024 * 1024;
	} else
		g_warning("Could not read key '%s' in buffer '%s'",
			  key, buffer);

	return value;
}


char *
skip_token (const char *p)
{
	p = next_token(p);
	while (*p && !isspace(*p)) p++;
	p = next_token(p);
	return (char *)p;
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

	buffer [0] = '\0';

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




static unsigned long
read_boot_time(glibtop *server)
{
	char buffer[BUFSIZ];
	char *btime;

	file_to_buffer(server, buffer, "/proc/stat");

	btime = strstr(buffer, "btime");

	if (!btime) {
		glibtop_warn_io_r(server, "cannot find btime in /proc/stat");
		return 0UL;
	}

	btime = skip_token(btime);
	return strtoul(btime, NULL, 10);
}



unsigned long
get_boot_time(glibtop *server)
{
	static unsigned long boot_time = 0UL;

	if(G_UNLIKELY(!boot_time))
	{
		boot_time = read_boot_time(server);
	}

	return boot_time;
}


size_t
get_page_size(void)
{
	static size_t pagesize = 0;

	if(G_UNLIKELY(!pagesize))
	{
		pagesize = getpagesize();
	}

	return pagesize;
}



gboolean
check_cpu_line(glibtop *server, const char *line, unsigned i)
{
	char start[10];

	g_snprintf(start, sizeof start, "cpu%u", i);

	return g_str_has_prefix(line, start);
}

