
AM_CPPFLAGS			= @AM_CPPFLAGS@

noinst_LTLIBRARIES		= libgtop_sysdeps-2.0.la libgtop_sysdeps_suid-2.0.la

libgtop_sysdeps_2_0_la_SOURCES	= nosuid.c siglist.c sysinfo.c

libgtop_sysdeps_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtop_sysdeps_suid_2_0_la_LIBADD	= $(KVM_LIBS)
libgtop_sysdeps_suid_2_0_la_SOURCES	= open.c close.c cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c disk.c procaffinity.c procio.c \
				  proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c procargs.c procmap.c netlist.c \
				  netload.c ppp.c procopenfiles.c fsusage.c

libgtop_sysdeps_suid_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtopinclude_HEADERS		= glibtop_server.h glibtop_machine.h \
				  glibtop_suid.h

libgtopincludedir		= $(includedir)/libgtop-2.0
