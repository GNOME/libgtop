#include <config.h>
#include <glibtop/procargs.h>

#include <glib.h>
#include <string.h>

/*
  Splitting args0
  * args0 [......0..   ....0.........0]
  * Returned value must be free with g_strfreev
  * args0 is g_free()d
  */
static char **
split_args0(const char * const args0, size_t size)
{
	GPtrArray *argv = g_ptr_array_new();
	const char *arg;

	/* remember that if there were no args, args0 == NULL and size == 0 */
	for (arg = args0; arg < (args0 + size); arg += strlen(arg) + 1) {
		g_ptr_array_add(argv, g_strdup(arg));
	}

	g_ptr_array_add(argv, NULL);

	g_free((void* /* remove constness */) args0);

	return (char**) g_ptr_array_free(argv, FALSE);
}




/*
 * public functions
 */

char **
glibtop_get_proc_argv_l (glibtop *server, glibtop_proc_args *buf,
			 pid_t pid, unsigned max_len)
{
	const char * const args0 = glibtop_get_proc_args_l(server, buf, pid, max_len);
	return split_args0(args0, buf->size);
}
