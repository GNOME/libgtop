LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

lib_LTLIBRARIES			= libgtop_sysdeps.la

libgtop_sysdeps_la_SOURCES	= open.c close.c kernel.s cpu.c mem.c swap.c \
				  uptime.c loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c siglist.c procargs.c procmap.c \
				  netload.c ppp.c

libgtop_sysdeps_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtopinclude_HEADERS		= glibtop_server.h
libgtopincludedir		= $(includedir)/libgtop-1.0

noinst_HEADERS			= kernel.h

kernel.o kernel.lo: $(srcdir)/kernel.s
	rm -f kernel.o kernel.lo
	$(AS) -o kernel.o $(srcdir)/kernel.s
	cp kernel.o kernel.lo

