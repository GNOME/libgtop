LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

if INODEDB
inodedb_SRCLIST			= inodedb.c
inodedb_DEFS			= -DSYSTEM_INODEDB=\"$(prefix)/var/libgtop/inodedb.db\"
else
inodedb_SRCLIST			=
inodedb_DEFS			=
endif

CFLAGS				= @CFLAGS@ $(inodedb_DEFS)

lib_LTLIBRARIES			= libgtop_common.la libgtop_suid_common.la

libgtop_common_la_SOURCES	= xmalloc.c error.c gnuslib.c \
				  fsusage.c fsusage.h mountlist.c mountlist.h \
				  $(inodedb_SRCLIST)

libgtop_common_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_common_la_LIBADD	= $(LIBGTOP_EXTRA_LIBS)

libgtop_suid_common_la_SOURCES	= xmalloc.c error.c sysdeps_suid.c

libgtop_suid_common_la_LDFLAGS	= $(LT_VERSION_INFO)

EXTRA_DIST			= inodedb.c
