LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @GTOP_INCS@

CFLAGS				= -Wall -W @CFLAGS@

lib_LTLIBRARIES			= libgtop_common.la

libgtop_common_la_SOURCES	= sysdeps.c xmalloc.c error.c

