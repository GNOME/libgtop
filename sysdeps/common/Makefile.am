LINK			= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES		= @INCLUDES@

backenddir		= @LIBGTOP_BACKEND_DIR@

backend_DATA		= libgtop-common.backend

backend_LTLIBRARIES	= libgtop_backend_common.la

EXTRA_DIST		= $(backend_DATA)

libgtop_backend_common_la_SOURCES = \
	backend.c fsusage.c mountlist.c

libgtop_backend_common_la_LIBADD = \
	@GLIB_LIBS@

libgtop_backend_common_la_LDFLAGS = \
	$(LT_VERSION_INFO)

