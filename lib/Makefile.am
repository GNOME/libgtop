LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @GTOP_INCS@

CFLAGS				= -Wall -W @CFLAGS@ -DGTOP_SERVER=\""@GTOP_SERVER@"\"

lib_LTLIBRARIES			= libgtop.la

libgtop_la_SOURCES		= init.c open.c close.c read.c read_data.c \
				  write.c command.c cpu.c mem.c swap.c uptime.c \
				  loadavg.c shm_limits.c msg_limits.c \
				  sem_limits.c proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c prockernel.c \
				  procsegment.c

