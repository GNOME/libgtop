#include <glibtop.h>
#include <glibtop/sysinfo.h>

static void
test_simple_cpuinfo (void)
{
  glibtop_sysinfo *infos = glibtop_get_sysinfo ();

  g_assert_cmpint (infos->ncpu, ==, 4);

}
