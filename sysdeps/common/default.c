#include <config.h>

#include <glibtop.h>
#include <glibtop/sysinfo.h>
#include <glibtop/union.h>


/**
 * glibtop_close:
 *
 * Close the connection to the server.
 */
void
glibtop_close(void)
{
	glibtop_close_r(glibtop_global_server);
}

/**
 * SECTION:glibtop
 * @short_description: Server initilization
 * @stability: Stable
 *  
 * You do not need to worry about the #glibtop * server structure if
 * you don't need - the library exports a #glibtop_global_server
 * which you can use everywhere a #glibtop * is expected.
 *
 * Most of the library and all of the sysdeps function also have an alias
 * (which is the function name without the <suffix>_l</suffix>, 
 * <suffix>_s</suffix> or <suffix>_r</suffix> suffix) which don't 
 * take a #glibtop * as argument but uses the #glibtop_global_server 
 * instead.
 */

/**
 * glibtop_init:
 *
 * Server initialization.
 *
 * Returns: A #glibtop reference.
 */
glibtop*
glibtop_init(void)
{
	return glibtop_init_r(&glibtop_global_server, 0, 0);
}


/**
 * glibtop_get_cpu:
 * @buf: A location to return the CPU usage.
 *
 * Get the CPU usage.
 *
 * All CPU units are measured in <type>jiffies</type> which are normally
 * 1/100th of a second (in which case <type>frequency</type> equals 100),
 * but can also be in any other unit. To get seconds, divide them by 
 * <type>frequency</type>.
 */
void
glibtop_get_cpu(glibtop_cpu *buf)
{
	glibtop_get_cpu_l(glibtop_global_server, buf);
}


/**
 * glibtop_get_disk:
 * @buf: A location to return the disk usage.
 *
 * Get the DISK usage.
 *
 */
void
glibtop_get_disk (glibtop_disk *buf)
{
	glibtop_get_disk_l (glibtop_global_server, buf);
}


/**
 * glibtop_get_fsusage:
 * @buf: A location to return the file system usage.
 * @mount_dir: mount dir where to get the information of usage.
 *
 * Get the file system usage for an specific @mount_dir.
 */
void
glibtop_get_fsusage(glibtop_fsusage *buf, const char *mount_dir)
{
	glibtop_get_fsusage_l(glibtop_global_server, buf, mount_dir);
}


/**
 * glibtop_get_uptime:
 * @buf: A location to return the system uptime
 *
 * When porting LibGTop to a new system, you only need to implement 
 * #uptime and #idletime if there's a faster or better way to obtain them
 * as using function(glibtop_cpu) for it. Look at 
 * <filename>sysdeps/freebsd/uptime.c</filename> for an
 * example on how to obtain them using function(glibtop_cpu).
 */
void
glibtop_get_uptime(glibtop_uptime *buf)
{
	glibtop_get_uptime_l(glibtop_global_server, buf);
}


/**
 * glibtop_sysinfo:
 *
 * Returns: The system information through a #glibtop_sysinfo structure.
 */
const glibtop_sysinfo *
glibtop_get_sysinfo(void)
{
	return glibtop_get_sysinfo_s(glibtop_global_server);
}


/**
 * glibtop_get_swap:
 * @buf: A location to return a #glibtop_swap.
 *
 * Get the swap usage.
 */
void
glibtop_get_swap(glibtop_swap *buf)
{
	glibtop_get_swap_l(glibtop_global_server, buf);
}


/**
 * glibtop_get_proc_uid:
 * @buf: A location to return a #glibtop_proc_uid
 * @pid: Process id to get the user and tty information
 * 
 * Get the process user id and tty information.
 */
void
glibtop_get_proc_uid(glibtop_proc_uid *buf, pid_t pid)
{
	glibtop_get_proc_uid_l(glibtop_global_server, buf, pid);
}


/**
 * SECTION:proctime
 * @title: Process Time
 * @short_description: Get process time information
 * @stability: Stable
 */

/**
 * glibtop_get_proc_time:
 * @buf: Returned process time information - see #glibtop_proc_time.
 * @pid: Process id
 *
 * Get process time information.
 */
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


/**
 * SECTION:procargs
 * @title: Process Arguments
 * @short_description: Get process command line arguments
 * @see_also: #libgtop-Process-List
 * @stability: Stable
 */

/**
 * glibtop_get_proc_args:
 * @buf: Struct with @size of returned string.
 * @pid: Process id
 * @max_len: Maximum length of string to return (use zero to get all arguments).
 *
 * Get process command line arguments.
 *
 * Returns: @pid's command line arguments separated by null bytes; the length of 
 * this string is returned in the @buf size field.  You are required to free
 * the string when done.
 */
char *
glibtop_get_proc_args(glibtop_proc_args *buf, pid_t pid, unsigned max_len)
{
	return glibtop_get_proc_args_l(glibtop_global_server, buf, pid, max_len);
}


/**
 * glibtop_get_proc_argv
 * @buf: Struct with @size of combined returned arguments.
 * @pid: Process id
 * @max_len: Maximum length of all arguments combined (use zero to get all arguments).
 *
 * Get process command line arguments.
 *
 * Returns: A NULL-terminated array of strings with all arguments of process pid
 * (up to @max_len characters).  Remember to <function>g_strfreev</function>
 * the returned array to avoid a memory leak.
 */
char **
glibtop_get_proc_argv(glibtop_proc_args *buf, pid_t pid, unsigned max_len)
{
	return glibtop_get_proc_argv_l(glibtop_global_server, buf, pid, max_len);
}


/**
 * SECTION:proclist
 * @title: Process List
 * @short_description: List running processes
 * @stability: Stable
 */

/**
 * glibtop_get_proclist
 * @buf: Extra return information, see #glibtop_proclist.
 * @which: Criteria for processes in returned list.  See the <type>GLIBTOP_KERN_PROC_*</type> and <type>GLIBTOP_EXCLUDE_*</type> constants.
 * @arg: Extra arguments applied to @which.  Only <type>GLIBTOP_KERN_PROC_*</type> constants take arguments, see each constant definition for particular @arg description.
 *
 * Returns: A list of running processes or <type>NULL</type> on error.  The
 * returned list is allocated using <function>g_malloc</function> and must be
 * freed using <function>g_free</function> to avoid a memory leak.
 */
pid_t*
glibtop_get_proclist(glibtop_proclist *buf, gint64 which, gint64 arg)
{
	return glibtop_get_proclist_l(glibtop_global_server, buf, which, arg);
}


void
glibtop_get_proc_kernel(glibtop_proc_kernel *buf, pid_t pid)
{
	glibtop_get_proc_kernel_l(glibtop_global_server, buf, pid);
}


/**
 * SECTION:ppp
 * @short_description: PPP Usage.
 * @see_also: #libgtop-netload, #libgtop-netlist
 * @stability: Stable
 *
 * Management of a PPP device.
 */

/**
 * glibtop_get_ppp:
 * @buf: A location to return the PPP usage
 * @short device: The device to ask information
 *
 * Get the PPP usage.
 */
void
glibtop_get_ppp(glibtop_ppp *buf, unsigned short device)
{
	glibtop_get_ppp_l(glibtop_global_server, buf, device);
}

/**
 * SECTION:netlist
 * @short_description: Network Devices List.
 * @see_also: #libgtop-netload
 * @stability: Stable
 *
 * The application class handles ...
 */

/**
 * glibtop_get_nelist:
 * @buf:
 *
 * Get the list of network devices.
 *
 * Returns: A list of network devices.
 */
char**
glibtop_get_netlist(glibtop_netlist *buf)
{
	return glibtop_get_netlist_l(glibtop_global_server, buf);
}

/**
 * SECTION:netload
 * @short_description: Network Load.
 * @see_also: #libtop-netlist
 * @stability: Stable
 *
 * The application class handles ...
 */


/**
 * glibtop_get_netload:
 * @buf: The variable where the results will be assigned.
 * @interface: The name of the network interface.
 *
 * Recolects network statistics for @interface
 * (which is the same than in <application>ifconfig</application>).  
 * The values are returned into @buf.
 */ 
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


/**
 * glibtop_get_mem:
 * @buf: Buffer where the output will be given.
 *
 * Get the memory usage. Unless explicitly stated otherwise, all memory
 * units are in bytes.
 */
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


/**
 * glibtop_get_proc_wd:
 * @buf:
 * @pid: Process id to get the user and tty information
 *
 * Get the root directory and the working directories
 *
 * Returns: A NULL-terminated list of working directories.
 */
char**
glibtop_get_proc_wd(glibtop_proc_wd *buf, pid_t pid)
{
	return glibtop_get_proc_wd_l(glibtop_global_server, buf, pid);
}


/**
 * glibtop_get_proc_affinity:
 * @buf:
 * @pid: Process id to get the affinity
 *
 * Get the processor affinity list for a process
 *
 * Returns: A list of processor ID of 'buf.number' elements.
 */
guint16 *
glibtop_get_proc_affinity(glibtop_proc_affinity *buf, pid_t pid)
{
	return glibtop_get_proc_affinity_l(glibtop_global_server, buf, pid);
}

/**
 * glibtop_get_proc_io: Get the io stats for the given pid
 * @buf: Buffer where the result will be given
 * @pid: Process id to get the io stats for
 *
 * Get the io stats for a process
 *
 * Returns: A list of processor ID of 'buf.number' elements.
 */
void
glibtop_get_proc_io(glibtop_proc_io *buf, pid_t pid)
{
	return glibtop_get_proc_io_l(glibtop_global_server, buf, pid);
}
