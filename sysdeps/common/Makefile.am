LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_common.la libgtop_suid_common.la

libgtop_common_la_SOURCES	= xmalloc.c error.c gnuslib.c \
				  fsusage.c fsusage.h mountlist.c mountlist.h

libgtop_suid_common_la_SOURCES	= xmalloc.c error.c
