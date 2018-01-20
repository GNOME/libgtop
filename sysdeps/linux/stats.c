#include <stdio.h>
#include <glibtop/stats.h>
#include <stdlib.h>

void 
stat_init(stat_entry *st, char *proc_name, char *device_name, double recv_value, double sent_value, pid_t pid, uid_t uid)
{
	st->proc_name = proc_name;
	st->device_name = device_name;
	st->pid = pid;
	st->uid = uid;
	st->sent_value = sent_value;
	st->recv_value = recv_value;
	g_assert(pid >= 0);
}

void
print_entry(stat_entry *st)
{
	printf("pid :%d \t sent_value:%f \trecv value:%f \tname of process:%s device_name:%s\n", st->pid, st->sent_value, st->recv_value, st->proc_name, st->device_name);
}

void
print_stat(stat_entry *st, int nproc)
{
	for(int i = 0; i < nproc; i++)
	{
		print_entry(&st[i]);
	}
	free(st);
}
