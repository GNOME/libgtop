#include <string.h>
#include <stdlib.h>

unsigned long long
get_scaled(const char *buffer, const char *key)
{
	const char    *ptr;
	char	      *next;
	const size_t len = strlen(key);
	unsigned long long value = 0;

	if ((ptr = strstr(buffer, key)))
	{
		ptr += len;
		value = strtoull(ptr, &next, 0);
		if (strchr(next, 'k'))
			value *= 1024;
		else if (strchr(next, 'M'))
			value *= 1024 * 1024;
	}
	return value;
}

