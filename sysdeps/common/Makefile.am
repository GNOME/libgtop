LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

INCLUDES			= @INCLUDES@

dynamic_ldflags			= @libgtop_dynamic_ldflags@

if INODEDB
inodedb_SRCLIST			= inodedb.c
inodedb_DEFS			= -DSYSTEM_INODEDB=\"$(prefix)/var/libgtop/inodedb.db\"
else
inodedb_SRCLIST			=
inodedb_DEFS			=
endif

CFLAGS				= @CFLAGS@ $(inodedb_DEFS)

noinst_LTLIBRARIES		= libgtop_common.la libgtop_suid_common.la

libgtop_common_la_SOURCES	= fsusage.c fsusage.h mountlist.c mountlist.h \
				  $(inodedb_SRCLIST)

libgtop_common_la_LIBADD	= $(LIBGTOP_EXTRA_LIBS)

libgtop_suid_common_la_SOURCES	= xmalloc_suid.c error_suid.c sysdeps_suid.c

EXTRA_DIST			= inodedb.c
