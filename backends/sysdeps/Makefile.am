LINK		= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES	= @INCLUDES@

noinst_HEADERS = \
	glibtop-backend-private.h

lib_LTLIBRARIES = \
	libgtop_backend_sysdeps.la

libgtop_backend_sysdeps_la_SOURCES = \
	backend-sysdeps.c \
	call-vector.c

libgtop_backend_sysdeps_la_LDFLAGS = \
	$(LT_VERSION_INFO)

