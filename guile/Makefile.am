LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

sitedir = $(datadir)/guile/site
libgtopmoduledir = $(datadir)/guile/libgtop

libgtopmodule_LTLIBRARIES	= libgtop.la libnames.la

libgtop_la_SOURCES		= dynamic.c

libnames_la_SOURCES		= dynamic_names.c

libgtop_la_LIBADD		= ../sysdeps/guile/guile.lo \
				  ../lib/init.lo \
				  ../lib/open.lo \
				  ../lib/close.lo \
				  ../lib/command.lo \
				  ../lib/read.lo \
				  ../lib/read_data.lo \
				  ../lib/write.lo \
				  ../lib/lib.lo \
				  ../lib/parameter.lo \
				  ../lib/sysdeps.lo \
				  ../sysdeps/common/error.lo \
				  ../sysdeps/common/fsusage.lo \
				  ../sysdeps/common/gnuslib.lo \
				  ../sysdeps/common/mountlist.lo \
				  ../sysdeps/common/xmalloc.lo \
				  ../sysdeps/@sysdeps_dir@/close.lo \
				  ../sysdeps/@sysdeps_dir@/proclist.lo \
				  ../sysdeps/@sysdeps_dir@/sem_limits.lo \
				  ../sysdeps/@sysdeps_dir@/cpu.lo \
				  ../sysdeps/@sysdeps_dir@/procmem.lo \
				  ../sysdeps/@sysdeps_dir@/shm_limits.lo \
				  ../sysdeps/@sysdeps_dir@/loadavg.lo \
				  ../sysdeps/@sysdeps_dir@/procsegment.lo \
				  ../sysdeps/@sysdeps_dir@/siglist.lo \
				  ../sysdeps/@sysdeps_dir@/mem.lo \
				  ../sysdeps/@sysdeps_dir@/procsignal.lo \
				  ../sysdeps/@sysdeps_dir@/swap.lo \
				  ../sysdeps/@sysdeps_dir@/msg_limits.lo \
				  ../sysdeps/@sysdeps_dir@/procstate.lo \
				  ../sysdeps/@sysdeps_dir@/uptime.lo \
				  ../sysdeps/@sysdeps_dir@/open.lo \
				  ../sysdeps/@sysdeps_dir@/proctime.lo \
				  ../sysdeps/@sysdeps_dir@/prockernel.lo \
				  ../sysdeps/@sysdeps_dir@/procuid.lo \
				  ../sysdeps/@sysdeps_dir@/procmap.lo

libnames_la_LIBADD		= ../sysdeps/guile/names/guile-names.lo

