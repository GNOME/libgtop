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
	const char    *ptr = buffer;
	char	      *next;
	unsigned long long value;

	if (key) {
		if (G_LIKELY((ptr = strstr(buffer, key))))
			ptr += strlen(key);
		else {
			g_warning("Could not read key '%s' in buffer '%s'",
				  key, buffer);
			return 0;
		}
	}

	value = strtoull(ptr, &next, 0);

	for ( ; *next; ++next) {
		if (*next == 'k') {
			value *= 1024;
			break;
		} else if (*next == 'M') {
			value *= 1024 * 1024;
			break;
		}
	}

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

/*
 * Doesn't handle bufsiz == 0
 */
int try_file_to_buffer(char *buffer, size_t bufsiz, const char *format, ...)
{
	char path[4096];
	int fd;
	size_t len = 0;
	ssize_t nread = 0;
	va_list pa;

	if (G_UNLIKELY(bufsiz <= sizeof(char*)))
	  g_warning("Huhu, bufsiz of %lu looks bad", (gulong)bufsiz);

	va_start(pa, format);

	/* C99 also provides vsnprintf */
	g_vsnprintf(path, sizeof path, format, pa);

	va_end(pa);

	bufsiz--; /* reserve 1 for trailing NUL */
	buffer [0] = '\0';

	if((fd = open (path, O_RDONLY)) < 0)
		return TRY_FILE_TO_BUFFER_OPEN;

	while (len < bufsiz) {
		nread = read (fd, buffer + len, bufsiz - len);

		if (G_UNLIKELY(nread < 0)) {
			if (errno == EINTR)
				continue;
			else
				break;
		}

		len += nread;

		if (nread == 0)
			break;
	}

	close (fd);

	if (nread < 0)
		return TRY_FILE_TO_BUFFER_READ;

	buffer [len] = '\0';

	return TRY_FILE_TO_BUFFER_OK;
}


void
file_to_buffer(glibtop *server, char *buffer, size_t bufsiz, const char *filename)
{
	switch(try_file_to_buffer(buffer, bufsiz, "%s", filename))
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
	char* line = NULL;
	size_t size = 0;
	FILE* stat;
	unsigned long btime = 0;

	if (!(stat = fopen("/proc/stat", "r"))) {
		glibtop_error_io_r(server, "fopen(\"/proc/stat\")");
		goto out;
	}

	while (getline(&line, &size, stat) != -1) {
		if (!strncmp(line, "btime", 5)) {
			btime = strtoul(skip_token(line), NULL, 10);
			break;
		}
	}

	free(line);
	fclose(stat);
out:
	return btime;
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



gboolean safe_readlink(const char *path, char *buf, size_t bufsiz)
{
	ssize_t ret;

	ret = readlink(path, buf, bufsiz - 1);

	if (ret == -1) {
		g_warning("Could not read link %s : %s", path, strerror(errno));
		return FALSE;
	}

	buf[ret] = '\0';
	return TRUE;
}
