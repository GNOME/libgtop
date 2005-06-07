#include <config.h>

#include <glibtop.h>
#include <glibtop/sysinfo.h>
#include <glibtop/union.h>


void
glibtop_close(void)
{
	glibtop_close_r(glibtop_global_server);
}


glibtop*
glibtop_init(void)
{
	return glibtop_init_r(&glibtop_global_server, 0, 0);
}


void
glibtop_get_cpu(glibtop_cpu *buf)
{
	glibtop_get_cpu_l(glibtop_global_server, buf);
}


void
glibtop_get_fsusage(glibtop_fsusage *buf, const char *mount_dir)
{
	glibtop_get_fsusage_l(glibtop_global_server, buf, mount_dir);
}


void
glibtop_get_uptime(glibtop_uptime *buf)
{
	glibtop_get_uptime_l(glibtop_global_server, buf);
}


const glibtop_sysinfo *
glibtop_get_sysinfo(void)
{
	return glibtop_get_sysinfo_s(glibtop_global_server);
}


void
glibtop_get_swap(glibtop_swap *buf)
{
	glibtop_get_swap_l(glibtop_global_server, buf);
}


void
glibtop_get_proc_uid(glibtop_proc_uid *buf, pid_t pid)
{
	glibtop_get_proc_uid_l(glibtop_global_server, buf, pid);
}


void
glibtop_get_proc_time(glibtop_proc_time *buf, pid_t pid)
{
	glibtop_get_proc_time_l(glibtop_global_server, buf, pid);
}


void
glibtop_get_proc_state(glibtop_proc_state *buf, pid_t pid)
{
	glibtop_get_proc_state_l(glibtop_global_server, buf, pid);
}


void
glibtop_get_proc_signal(glibtop_proc_signal *buf, pid_t pid)
{
	glibtop_get_proc_signal_l(glibtop_global_server, buf, pid);
}


void
glibtop_get_proc_segment(glibtop_proc_segment *buf, pid_t pid)
{
	glibtop_get_proc_segment_l(glibtop_global_server, buf, pid);
}


glibtop_open_files_entry *
glibtop_get_proc_open_files(glibtop_proc_open_files *buf, pid_t pid)
{
	return glibtop_get_proc_open_files_l(glibtop_global_server, buf, pid);
}


void
glibtop_get_proc_mem(glibtop_proc_mem *buf, pid_t pid)
{
	glibtop_get_proc_mem_l(glibtop_global_server, buf, pid);
}


glibtop_map_entry *
glibtop_get_proc_map(glibtop_proc_map *buf, pid_t pid)
{
	return glibtop_get_proc_map_l(glibtop_global_server, buf, pid);
}


char *
glibtop_get_proc_args(glibtop_proc_args *buf, pid_t pid, unsigned max_len)
{
	return glibtop_get_proc_args_l(glibtop_global_server, buf, pid, max_len);
}


char **
glibtop_get_proc_argv(glibtop_proc_args *buf, pid_t pid, unsigned max_len)
{
	return glibtop_get_proc_argv_l(glibtop_global_server, buf, pid, max_len);
}


unsigned *
glibtop_get_proclist(glibtop_proclist *buf, gint64 which, gint64 arg)
{
	return glibtop_get_proclist_l(glibtop_global_server, buf, which, arg);
}


void
glibtop_get_proc_kernel(glibtop_proc_kernel *buf, pid_t pid)
{
	glibtop_get_proc_kernel_l(glibtop_global_server, buf, pid);
}


void
glibtop_get_ppp(glibtop_ppp *buf, unsigned short device)
{
	glibtop_get_ppp_l(glibtop_global_server, buf, device);
}


char**
glibtop_get_netlist(glibtop_netlist *buf)
{
	return glibtop_get_netlist_l(glibtop_global_server, buf);
}


void
glibtop_get_netload(glibtop_netload *buf, const char *interface)
{
	glibtop_get_netload_l(glibtop_global_server, buf, interface);
}


glibtop_mountentry *
glibtop_get_mountlist(glibtop_mountlist *buf, int all_fs)
{
	return glibtop_get_mountlist_l(glibtop_global_server, buf, all_fs);
}


void
glibtop_get_mem(glibtop_mem *buf)
{
	glibtop_get_mem_l(glibtop_global_server, buf);
}


void
glibtop_get_loadavg(glibtop_loadavg *buf)
{
	glibtop_get_loadavg_l(glibtop_global_server, buf);
}


void
glibtop_get_msg_limits(glibtop_msg_limits *buf)
{
	glibtop_get_msg_limits_l(glibtop_global_server, buf);
}


void
glibtop_get_sem_limits(glibtop_sem_limits *buf)
{
	glibtop_get_sem_limits_l(glibtop_global_server, buf);
}


void
glibtop_get_shm_limits(glibtop_shm_limits *buf)
{
	glibtop_get_shm_limits_l(glibtop_global_server, buf);
}


void
glibtop_get_sysdeps(glibtop_sysdeps *buf)
{
	glibtop_get_sysdeps_r(glibtop_global_server, buf);
}




