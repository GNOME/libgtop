#include <glibtop.h>
#include <glibtop/mem.h>
#include <glibtop/swap.h>

#include <glib.h>
#include <unistd.h>

static unsigned mb(guint64 n)
{
        return n >> 20;
}

int main(int argc, char **argv)
{
        glibtop_mem mem;
        glibtop_swap swap;

	glibtop_init();

        glibtop_get_mem(&mem);
        glibtop_get_swap(&swap);

        printf("              total        used        free      shared  buff/cache   available\n");

        printf("%-7s %11u %11u %11u %11u %11u %11u\n",
               "Mem:",
                mb(mem.total),
                mb(mem.used - mem.buffer - mem.cached),
                mb(mem.free),
                mb(mem.shared),
                mb(mem.buffer + mem.cached),
                mb(mem.total - mem.user));

        printf("%-7s %11u %11u %11u\n",
               "Swap:",
                mb(swap.total),
                mb(swap.used),
                mb(swap.free));

	glibtop_close();

	return 0;
}

